/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#include <irrlicht/irrlicht.h>
#include "TankNormal.h"
#include "GestionBruits.h"

using namespace irr;

#define PI 3.141592653589793
#define ADAPTER_VITESSE_CAMERA 180/PI/1000
#define MARGE 100

TankNormal::TankNormal(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver* driver,irr::gui::IGUIEnvironment* gui, irr::ITimer* timer, irr::scene::ITerrainSceneNode* terrain, irr::scene::IMeshSceneNode** paqueMunition, Client client, sf::IpAddress adresseServeur, unsigned short portServeur, GestionExplosion* gestionExplosion) : Joueur(sceneManager, driver, client, true, terrain)
{
    //RESEAU
    m_monID = client.id;
    m_adresseServeur = adresseServeur;
    m_portServeur = portServeur;

    //Partie importante de irrlich
    m_gui = gui;
    m_timer = timer;

    m_positionDeDepart.X = client.positionDepart;
    if(client.equipe)
    {
        m_canon->setRotation(core::vector3df(0,90,0));
        m_corps->setRotation(core::vector3df(0,90,0));
        m_positionDeDepart.Z = 10000;
    }

    else
    {
        m_canon->setRotation(core::vector3df(0,270,0));
        m_corps->setRotation(core::vector3df(0,270,0));
        m_positionDeDepart.Z = 1000;
    }
    m_positionDeDepart.Y = m_terrain->getHeight(m_positionDeDepart.X, m_positionDeDepart.Z);

    //Ressource (2d, son)
    //image
    m_imageStatutChargement = driver->getTexture("texture/munition.png");
    m_imageStatutChargementAucuneMunition = driver->getTexture("texture/aucuneMunition.png");
    m_barreSante = driver->getTexture("texture/coeur.png");
    m_barreDeSanteGrise = driver->getTexture("texture/coeurGris.png");

    m_soundBufferSonImpossible.loadFromFile("son/impossible.wav");
    m_sonImpossible.setBuffer(m_soundBufferSonImpossible);

    //Variable liée au stat du tank
    m_nbDeVie = 100;
    m_vitesse = 30;
    m_rotateSpeedCanon = 30;
    m_rotateSpeedCorps = 90;
    m_vitesseObus = 600;
    m_tempsDeRechargement = 2000;
    m_dernierTire = 0;
    m_nombreDeMunition = 5;
    m_nombreMaxDeMunition = 5;

    //Autre
    m_heureDuDeces = 0;
    m_onEstMort = false;
    m_gestionExplosion = gestionExplosion;
    m_paqueMunition = paqueMunition;
    m_texteMunition = m_gui->addStaticText(L"", core::rect<s32>(50,50,70,70), false, true, 0, 100);
}

void TankNormal::avancer(bool reculer)
{
    m_gestionBruits->jouerSon(m_monID, CHANGEMENT_POSITION);

    //on avance dans le sens de la rotation de corps
    core::vector3df direction = m_corps->getRotation().rotationToDirection() * m_vitesse;
    if(reculer)
        direction = -direction;

    m_corps->setPosition(m_corps->getPosition() + direction);
}

void TankNormal::tournerCanon(bool gauche)
{
    //si on tourne a gaucheajusterPositionRotation
    if(gauche)
    {
        m_canon->setRotation(m_canon->getRotation() + core::vector3df(0,-m_rotateSpeedCanon * ADAPTER_VITESSE_CAMERA,0));
    }
    //si on tourne a droite
    else
         m_canon->setRotation(m_canon->getRotation() + core::vector3df(0,m_rotateSpeedCanon * ADAPTER_VITESSE_CAMERA,0));
}

void TankNormal::tournerCorps(bool gauche)
{
    if(gauche)
    {
        m_corps->setRotation(m_corps->getRotation() + core::vector3df(0, 0.05 * -m_rotateSpeedCorps, 0));
    }
    else
         m_corps->setRotation(m_corps->getRotation() + core::vector3df(0, 0.05 * m_rotateSpeedCorps, 0));
}

void TankNormal::ajusterPositionRotation()
{
    //variable qu on va souvent utiliser
    core::vector3df positionCorps = m_corps->getPosition();
    core::vector3df rotationCorps = m_corps->getRotation();


    //on evite de sortir de la map ;)
    if(positionCorps.X > 11800)
        positionCorps.X = 11800;
    if(positionCorps.X < 200)
        positionCorps.X = 200;
    if(positionCorps.Z > 11800)
        positionCorps.Z = 11800;
    if(positionCorps.Z < 200)
        positionCorps.Z = 200;

    //on met le node a terre
    m_corps->setPosition(core::vector3df(positionCorps.X, m_terrain->getHeight(positionCorps.X, positionCorps.Z), positionCorps.Z));

    //on incline le mesh suivant le terrrin
    //variable
    core::vector3df pointAvant;
    core::vector3df pointArriere;
    //avant
    pointAvant = positionCorps + rotationCorps.rotationToDirection() * 100;
    pointAvant.Y = m_terrain->getHeight(pointAvant.X, pointAvant.Z);
    //arrietre
    pointArriere = positionCorps + rotationCorps.rotationToDirection() * -100;
    pointArriere.Y = m_terrain->getHeight(pointArriere.X, pointArriere.Z);

    //on trouvve le vecteur reprensentant l  angel d inclinaison et l angle de comparaison(Y = 0) pour trouver li ncinaison
    core::vector3df vecteurInclinaison, vecteurReference;
    vecteurInclinaison = pointAvant - pointArriere;

    vecteurReference = vecteurInclinaison;
    vecteurReference.Y = 0;

    //on les normalise
    vecteurInclinaison.normalize();
    vecteurReference.normalize();

    //on obtientt l angle de difference avec le produit scalair(WTF)
    double dot = vecteurInclinaison.X * vecteurReference.X + vecteurInclinaison.Y * vecteurReference.Y + vecteurInclinaison.Z * vecteurReference.Z;
    double angleInclinaison = acos(dot) * 180/PI;
    //si le nombre fait de la merde(WTF)
    if(isnan(angleInclinaison))
        angleInclinaison = 0;
    //si angle negatif
    if(vecteurInclinaison.Y > 0)
        angleInclinaison = -angleInclinaison;

    //on aplique les rotations
    m_corps->setRotation(core::vector3df(angleInclinaison,rotationCorps.Y,0));

    //et on met le canon a la position du corps (toutes les modif sont faites sur le corps on recopie juste)
    m_canon->setPosition(m_corps->getPosition());
}

bool TankNormal::essaiTire()
{
    //on prend le temps actuel
    irr::f32 tpsActu = m_timer->getTime();
    //si on a attendu asser avoir  rechargé
    if(tpsActu > m_dernierTire + m_tempsDeRechargement && m_nombreDeMunition > 0 && !m_onEstMort)
    {
        //on dit aux autre joueurs qu on fait un tire
        sf::Packet packet;
        packet<<m_monID<<TIRE;
        m_socket.send(packet, m_adresseServeur, m_portServeur);
        //on perd une munition
        m_nombreDeMunition--;
        //on actualise l heure le dernier tire
        m_dernierTire = tpsActu;

        return true;
    }

    // si on a plus de munition
    if(m_nombreDeMunition == 0)
    {
        //on joue le son "impossible"
        m_sonImpossible.play();
        //on recupere les dimension de l ecran pour mettre l indicatuer "impossible" au bon endroit
        m_driver->draw2DRectangle(video::SColor(250,250,0,0), core::rect<s32>(m_screenSize.Width - 200, m_screenSize.Height - 100, m_screenSize.Width - 50, m_screenSize.Height - 25));
    }
    return false;
}

void TankNormal::tire()
{
    //on joue le son tire
    m_gestionBruits->jouerSon(m_monID, TIRE);

    //FEU
    irr::scene::IParticleSystemSceneNode* ps = m_sceneManager->addParticleSystemSceneNode(false);
    //emeteur
    irr::scene::IParticleEmitter* em = ps->createBoxEmitter(core::aabbox3df(-2,0,-2,2,1,2), core::vector3df(0,0,0), 100,200, video::SColor(0,255,255,255), video::SColor(0,255,255,255), 50,100, 0, core::dimension2df(30,30), core::dimension2df(50,50));
    ps->setEmitter(em);
    //on oriente les particules
    em->setDirection(m_canon->getRotation().rotationToDirection());
    em->drop();
    //affector
    scene::IParticleAffector *paf = ps->createFadeOutParticleAffector(video::SColor(0,150,100,0), 100);
    ps->addAffector(paf);
    paf->drop();
    //reglage du m_ps
    ps->setMaterialTexture(0, m_driver->getTexture("texture/fire.bmp"));
    ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

    // on trouve le bout du canaon
    core::vector3df boutCanon;
    boutCanon = m_canon->getRotation().rotationToDirection();
    boutCanon.X *= 200;
    boutCanon.Z *= 200;
    boutCanon.Y += 65;
    boutCanon += m_canon->getPosition();
    //on y met le ps
    ps->setPosition(boutCanon);

    irr::scene::ISceneNodeAnimator* anm = m_sceneManager->createDeleteAnimator(50);
    ps->addAnimator(anm);
    anm->drop();

    //camera (variable utile pour la suite des operation)
    irr::scene::ICameraSceneNode* camera = m_sceneManager->getActiveCamera();
    //direction du tire
    core::vector3df direction;

    //ligne de collision
    core::line3df line;
    line.start = m_canon->getPosition();
    line.start.Y += 150; //(on se met a la hauteur du canon et pas du corps ;)
    line.end = camera->getPosition() + (camera->getTarget()-camera->getPosition())*20; //on recupere la ou on vise

    //si on tire dans le vide on met une direction aproximative
    direction = line.end - line.start;

                                //MAX   //MIN          //MIN
    f32 modificateurY = (rand() % (300 - (-300) + 1)) + (-300);
    f32 modificateurX = (rand() % (300 - (-300) + 1)) + (-300);
    f32 modificateurZ = (rand() % (300 - (-300) + 1)) + (-300);
    core::vector3df modificateur(modificateurX, modificateurY, modificateurZ);

    direction += modificateur;
    direction.normalize();

    Shoot newShoot(m_sceneManager, m_gestionExplosion, m_timer, m_collisionManager, m_terrainSelector, m_vitesseObus, direction, m_canon->getPosition(), m_listeAutreJoueurs);
    m_listeShoots.push_back(newShoot);
}

void TankNormal::rechargementMunition()
{

    //les position du tank
    f32 x = m_corps->getPosition().X;
    f32 z = m_corps->getPosition().Z;

    //on teste chaque paquet
    for(sf::Int32 i = 0 ; i<10 ; ++i)
    {
        f32 xMunition = m_paqueMunition[i]->getPosition().X;
        f32 zMunition = m_paqueMunition[i]->getPosition().Z;
        //si le paquet n a pas etait ramesse et qu on a pas toutes ses munition
        if(m_nombreDeMunition < m_nombreMaxDeMunition)
        {
            //on regarde si on est sur un pague
            if(x < xMunition + MARGE && x > xMunition - MARGE)
            {
                if(z < zMunition + MARGE && z > zMunition - MARGE)
                {
                    //si oui on joue le son de recharge
                    m_gestionBruits->jouerSon(m_monID, PACKET_MUNITION_CONSOMME);
                    //on regene les munition
                    m_nombreDeMunition = m_nombreMaxDeMunition;
                    //on bouge le paque
                    core::vector3df position;
                    //on tire les position x et z au pif
                    position.X = (rand() % (11500 - 500 + 1)) + 500;
                    position.Z = (rand() % (11500 - 500 + 1)) + 500;
                    //la position en y est la hautuer du terrain au position x et z
                    position.Y = m_terrain->getHeight(position.X, position.Z);
                    m_paqueMunition[i]->setPosition(position);

                    sf::Packet packet;
                    packet<<m_monID<<PACKET_MUNITION_CONSOMME<<i<<position;
                    m_socket.send(packet, m_adresseServeur, m_portServeur);
                }
            }
        }
    }
}

void TankNormal::majRechargementMunitionEtSante()
{
    //RIEN A VOIR AVEC LE NOM DE LA FONCION MAIS
    //ON PROFITE QU ELLE SOIT APPELLE REGULIEREMENT
    //POUR VERIFIER CERTAIN TRUC
    m_screenSize = m_driver->getScreenSize();

    if(m_onEstMort)
    {
        irr::s32 alpha = 10000 - (m_timer->getTime() - m_heureDuDeces);
        alpha = 255 * alpha / 10000;
        m_driver->draw2DRectangle(video::SColor(alpha,255,0,0), core::rect<s32>(0,0,m_driver->getScreenSize().Width, m_driver->getScreenSize().Height));

        if(m_timer->getTime() > m_heureDuDeces + 10000)
        {
            m_onEstMort = false;
            m_rotateSpeedCorps = 90;
        }
    }

    //barre de rechargementm_dansLaFenetreOption
    m_driver->draw2DRectangle(video::SColor(100,200,200,200), core::rect<s32>(m_screenSize.Width - 200, m_screenSize.Height - 100, m_screenSize.Width - 50, m_screenSize.Height - 25));

    // on prend le temps entre le dernier tire et maintenenant
    irr::f32 tpsActu = m_timer->getTime();
    irr::f32 difference = tpsActu - m_dernierTire;

    //si on a encore des munitions on met l image  de chargement a jou
    if(difference < m_tempsDeRechargement && m_nombreDeMunition > 0)
    {
        irr::s32 etapeRechargement = difference/13;
        m_driver->draw2DImage(m_imageStatutChargement, core::position2d<s32>(m_screenSize.Width - 200, m_screenSize.Height - 100) ,core::rect<s32>(0,0,etapeRechargement,75), 0, video::SColor(155,255,255,255));
    }

    else//si on en a plus on met l image "pas de munition"
    if(m_nombreDeMunition == 0)
    {
       m_driver->draw2DImage(m_imageStatutChargementAucuneMunition, core::position2d<s32>(m_screenSize.Width - 200, m_screenSize.Height - 100) ,core::rect<s32>(0,0,150,75), 0, video::SColor(200,255,255,255), true);
    }

    //sinon c est qu on peut tirer donc on met l image de rechargement pleine et pas transparente
    else
    {
        m_driver->draw2DImage(m_imageStatutChargement, core::position2d<s32>(m_screenSize.Width - 200, m_screenSize.Height - 100) ,core::rect<s32>(0,0,150,75), 0, video::SColor(255,255,255,255));
    }

    //on met a jour le texte in diquant le nombre de munition
    core::stringw texte;
    texte += m_nombreDeMunition;
    texte += "\\";
    texte += m_nombreMaxDeMunition;
    m_texteMunition->setText(texte.c_str());
    m_texteMunition->setRelativePosition(core::position2di(m_screenSize.Width-190, m_screenSize.Height-40));
    m_texteMunition->draw();

    m_driver->draw2DImage(m_barreDeSanteGrise, core::position2di(m_screenSize.Width/2 - 150, m_screenSize.Height - 110) , core::rect<s32>(0,0,300,100), 0, video::SColor(200,255,255,255), true);

    irr::s32 tailleImage = 300 * m_nbDeVie / 100;
    m_driver->draw2DImage(m_barreSante, core::position2di(m_screenSize.Width/2 - 150, m_screenSize.Height - 110) , core::rect<s32>(0,0,tailleImage,100), 0, video::SColor(255,255,255,255));
}

irr::scene::IMeshSceneNode* TankNormal::getCorps() const
{
    return m_corps;
}

void TankNormal::setRotateSpeedCanon(irr::f32 rotateSpeed)
{
    m_rotateSpeedCanon = rotateSpeed;
}

void TankNormal::adapterVitesseAuFps(irr::f32 coefficientFPS)
{
    if(!m_onEstMort)
    {
        m_vitesse = 30/coefficientFPS;
        m_rotateSpeedCorps = 90/coefficientFPS;
        m_vitesseObus = 600/coefficientFPS;
    }
}

irr::core::vector3df TankNormal::getRotCorps() const
{
    return m_corps->getRotation();
}

irr::core::vector3df TankNormal::getRotCanon() const
{
    return m_canon->getRotation();
}

void TankNormal::setListeAutreJoueur(std::vector<Joueur*> listeAutreJoueurs)
{
    m_listeAutreJoueurs = listeAutreJoueurs;

    //CREATION DE LA DETECTION DES COLLISION ENTRE LES AUTRE JOUEURS ET TOI
    //ON cree UN sekector pour chaque joueur
    scene::IMetaTriangleSelector *selector = m_sceneManager->createMetaTriangleSelector();
    for(std::vector<Joueur*>::iterator it = m_listeAutreJoueurs.begin(); it != m_listeAutreJoueurs.end(); ++it)
    {
        selector->addTriangleSelector(m_sceneManager->createTriangleSelector((*it)->getSceneNode()->getMesh(), (*it)->getSceneNode()));
    }

    //On cree un aniator de collision avec le selector
    scene::ISceneNodeAnimator *anm = m_sceneManager->createCollisionResponseAnimator(selector, m_corps, core::vector3df(80,50,80), core::vector3df(0,0,0), core::vector3df(0,0,0));
    m_corps->addAnimator(anm);
    selector->drop();
    anm->drop();
}

void TankNormal::setGestionBruits(GestionBruits* gestionBruits)
{
    m_gestionBruits = gestionBruits;
}

void TankNormal::setVolumeSonImpossibleTirer(irr::s32 volume)
{
    m_sonImpossible.setVolume(volume);
    m_sonImpossible.play();
}

void TankNormal::majShoots()
{
    for(std::vector<Shoot>::iterator it = m_listeShoots.begin(); it != m_listeShoots.end();)
    {
       if((*it).miseAJour())
       {
           sf::Packet packet;
            core::vector3df posExplo;
            posExplo = (*it).getPosition();
            packet<<m_monID<<EXPLOSION<<posExplo;
            m_socket.send(packet, m_adresseServeur, m_portServeur);
           it = m_listeShoots.erase(it);
       }

       else
       {
           ++it;
       }
    }
}

bool TankNormal::perdreDeLaSante()
{
    m_nbDeVie -= 20;

    if(m_nbDeVie == 0)
    {
        m_nombreDeMunition = 5;
        m_nbDeVie = 100;
        m_onEstMort = true;
        m_heureDuDeces = m_timer->getTime();
        m_vitesse = 0;
        m_rotateSpeedCorps = 0;
        m_corps->setPosition(m_positionDeDepart);
        m_canon->setPosition(m_corps->getPosition());
        return true;
    }

    else
    {
        return false;
    }
}

void TankNormal::cacherTextMunition()
{
    m_texteMunition->setVisible(false);
}

TankNormal::~TankNormal()
{
}
