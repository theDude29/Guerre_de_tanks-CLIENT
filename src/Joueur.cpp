/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#include "Joueur.h"

using namespace irr;

Joueur::Joueur(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver* driver, Client client, bool monJoueur, irr::scene::ITerrainSceneNode* terrain)
{
    //partie importante de irrlicht
    m_driver = driver;
    m_sceneManager = sceneManager;
    m_collisionManager = sceneManager->getSceneCollisionManager();

    //collision
    m_terrain = terrain;
    m_terrainSelector = sceneManager->createTerrainTriangleSelector(m_terrain);

    //partie du tank
    if(client.equipe)
    {
        m_corps = sceneManager->addMeshSceneNode(sceneManager->getMesh("mesh/tank.obj"), 0, -1, core::vector3df(client.positionDepart,500,10000));
        m_canon = sceneManager->addMeshSceneNode(sceneManager->getMesh("mesh/canon.obj"), 0, -1, m_corps->getPosition());
        m_corps->setScale(core::vector3df(50, 50, 50));
        m_canon->setScale(core::vector3df(50, 50, 50));
        m_corps->setMaterialTexture(0, driver->getTexture("texture/Main.tga"));
        m_canon->setMaterialTexture(0, driver->getTexture("texture/Main.tga"));
    }

    else
    {
        m_corps = sceneManager->addMeshSceneNode(sceneManager->getMesh("mesh/tank.obj"), 0, -1, core::vector3df(client.positionDepart,500,1000));
        m_canon = sceneManager->addMeshSceneNode(sceneManager->getMesh("mesh/canon.obj"), 0, -1, m_corps->getPosition());
        m_corps->setScale(core::vector3df(50, 50, 50));
        m_canon->setScale(core::vector3df(50, 50, 50));
        m_corps->setMaterialTexture(0, driver->getTexture("texture/Main2.tga"));
        m_canon->setMaterialTexture(0, driver->getTexture("texture/Main2.tga"));
    }

    //text pseudo
    m_pseudo = client.pseudo;
    if(!monJoueur)
    {
        m_camera = sceneManager->getActiveCamera();
        m_decalageDuText = strlen(m_pseudo.toAnsiString().c_str());
        m_decalageDuText /= 2;
        m_decalageDuText *= 8;
        m_textPseudo = sceneManager->getGUIEnvironment()->addStaticText(m_pseudo.toWideString().c_str(), core::rect<s32>(0, 0, m_decalageDuText * 2, 16));
    }

    //autre
    m_id = client.id;
}

void Joueur::miseAJour(infoPositionRotationJoueur info)
{
    m_corps->setPosition(info.pos);
    m_canon->setPosition(info.pos);
    m_corps->setRotation(info.rotCorps);
    m_canon->setRotation(info.rotCanon);
}

void Joueur::miseAJourPosTextPseudo(bool finDeLaPartie)
{
    if(finDeLaPartie)
    {
        m_textPseudo->remove();
    }

    else
    {
        core::vector3df posText3d = m_corps->getPosition();
        posText3d.Y += 130;

        core::position2di posText2d = m_collisionManager->getScreenCoordinatesFrom3DPosition(posText3d, m_camera);
        posText2d.X -= m_decalageDuText;
        m_textPseudo->setRelativePosition(posText2d);

        core::line3df line;
        line.start = m_camera->getPosition();
        line.end = m_corps->getPosition();
        line.end.Y += 50;
        core::vector3df v;
        core::triangle3df t;
        scene::ISceneNode* node = 0;
        if(m_collisionManager->getCollisionPoint(line, m_terrainSelector, v, t, node))
        {
            m_textPseudo->setVisible(false);
        }

        else
        {
            m_textPseudo->setVisible(true);
        }
    }
}

irr::scene::IMeshSceneNode* Joueur::getSceneNode()
{
    return m_corps;
}

core::vector3df Joueur::getPosition()
{
    return m_corps->getPosition();
}

void Joueur::tire()
{
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
}

core::stringw Joueur::getPseudo()
{
    core::stringw pseudo_wstr((m_pseudo.toWideString()).c_str());
    return pseudo_wstr;
}

Joueur::~Joueur()
{
}
