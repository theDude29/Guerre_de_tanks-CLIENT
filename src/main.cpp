#include "MyEventReceiver.h"
#include "MyEventReceiver2.h"
#include "decor.h"

using namespace irr;

int main()
{
    //On initialise le generateur de nombre aleatoire
    srand(time(NULL));

    //Parties importantes de irrlicht
    IrrlichtDevice* device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800,600), 32);
    scene::ISceneManager* sceneManager = device->getSceneManager();
    video::IVideoDriver* driver = device->getVideoDriver();
    gui::IGUIEnvironment* gui = device->getGUIEnvironment();
    //On change la police
    gui::IGUISkin *skin = gui->getSkin();
    skin->setFont(gui->getFont("fonthaettenschweiler.bmp"));
    //nom de la fenetre
    device->setWindowCaption(L"Guerre de Tank - Client");
    //music ambiance
    sf::Music music;
    music.openFromFile("son/IrrlichtTheme.ogg");
    music.setLoop(true);

    video::ITexture *titre, *sfml, *irrlicht;
    titre = driver->getTexture("texture/titre.png");
    driver->makeColorKeyTexture(titre, core::position2d<s32>(0,0));
    sfml = driver->getTexture("texture/logosfml.png");
    driver->makeColorKeyTexture(sfml, core::position2d<s32>(0,0));
    irrlicht = driver->getTexture("texture/logoirrlicht.png");

    core::dimension2du screenSize;

    music.play();

    //event receiver
    MyEventReceiver* receiver = new MyEventReceiver(gui, device->getTimer());
    device->setEventReceiver(receiver);

    while(device->run() && !receiver->miseAJour(screenSize))
    {
        driver->beginScene(true, true, video::SColor(255, 100, 100, 100));
        gui->drawAll();

        screenSize = driver->getScreenSize();
        driver->draw2DImage(titre, core::position2di(screenSize.Width/2 - 300, screenSize.Height/2 - 250), core::rect<s32>(0, 0, 550, 100), 0, video::SColor(255,255,255,255), true);
        driver->draw2DImage(irrlicht, core::position2di(30, screenSize.Height - 80));
        driver->draw2DImage(sfml, core::position2di(screenSize.Width - 300, screenSize.Height - 100), core::rect<s32>(0, 0, 270, 80), 0, video::SColor(255,255,255,255), true);

        driver->endScene();
    }

    music.stop();

    if(!device->run())
    {
        device->drop();
        delete receiver;
        return 0;
    }

    //ON COMENCE A JOUE
    sf::UdpSocket socket;
    socket.setBlocking(false);
    socket.bind(receiver->getPort());

    sf::IpAddress adresseServeur = receiver->getAdresseServeur();
    unsigned short portServeur = receiver->getPortServeur();

    sf::Packet packet;
    sf::IpAddress adresse;
    unsigned short port;

    sf::Uint32 monID = receiver->getID();
    sf::Uint32 IDautreJoueur = 0;

    std::vector<Client> listeClients = receiver->getListeClients();
    std::vector<Joueur*> listeToutsJoueurs, listeAutreJoueurs;

    //Reglage lumiere
    sceneManager->setAmbientLight(video::SColorf(1,1,1,1));

    //DECOR
    scene::ITerrainSceneNode* terrain = sceneManager->addTerrainSceneNode("texture/terrain-heightmap.bmp");
    scene::IMeshSceneNode* paqueMunition[10];
    creerDecor(sceneManager, driver, terrain, paqueMunition, receiver->getPosPaquesMunition());

    //GESTION EXPLOSION
    GestionExplosion gestionExplosion(sceneManager, driver, device->getTimer(), adresseServeur, portServeur, monID);

    //TANK
    bool monEquipe = listeClients[monID].equipe;
    TankNormal* tank = new TankNormal(sceneManager, driver, gui, device->getTimer(), terrain, paqueMunition, listeClients[monID], adresseServeur, portServeur, &gestionExplosion);

    //CAMERA
    scene::ICameraSceneNode* camera = sceneManager->addCameraSceneNode();
    camera->setFarValue(10000);
    MyCamera myCamera(camera, listeClients[monID].equipe);

    //Initialise les liste de joueurs
    for(unsigned int i = 0; i < listeClients.size(); ++i)
    {
        if(i == monID)
        {
            listeToutsJoueurs.push_back(tank);
        }

        else
        {
            Joueur* joueur = new Joueur(sceneManager, driver, listeClients[i], false, terrain);
            listeToutsJoueurs.push_back(joueur);
            listeAutreJoueurs.push_back(joueur);
        }
    }

    gestionExplosion.setMonJoueur(tank);
    tank->setListeAutreJoueur(listeAutreJoueurs);

    //GUI SPECIFIQUE A CE JEU
    MyGui myGui(device, driver, gui, tank, &myCamera);

    //GESTION DES SONS
    GestionBruits gestionBruits(listeToutsJoueurs, monID, &myGui);
    tank->setGestionBruits(&gestionBruits);
    myGui.setGestionBruits(&gestionBruits);
    gestionExplosion.setGestionBruits(&gestionBruits);

    //EVENT RECEIVER
    MyEventReceiver2* receiver2 = new MyEventReceiver2(sceneManager, driver, device->getTimer(), &myGui, tank, &myCamera, device->getCursorControl(), adresseServeur, portServeur, monID, &gestionBruits, receiver->getIntervalleRafraichissement(), receiver->getNbEliminationGagnePartie(), &socket);
    device->setEventReceiver(receiver2);
    delete receiver;

    //tmps pour recherche de manette
    irr::ITimer *timer = device->getTimer();
    u32 tempsActuelle = 0, derniereRechercheDeManette = 0;
    //info manette
    core::array<SJoystickInfo> joysticksInfo;
    //FPS
    core::stringw texteFps = "";
    s32 FPS = 0;

    infoPositionRotationJoueur infoAutreJoueur;
    bool arreter = false;
    while(device->run() && !arreter)
    {
        driver->beginScene(true, true, video::SColor(255, 100, 100, 100));
        sceneManager->drawAll();
        gui->drawAll();

        if(socket.receive(packet, adresse, port) == sf::Socket::Done)
        {
            packet>>IDautreJoueur;

            sf::Int32 typeMessage;
            packet>>typeMessage;

            switch(typeMessage)
            {
            case CHANGEMENT_POSITION_ROTATION:
                if(IDautreJoueur != monID)
                {
                    packet>>infoAutreJoueur;
                    listeToutsJoueurs[IDautreJoueur]->miseAJour(infoAutreJoueur);

                    bool aChangeDePositionEtPasQueDeRotation;
                    packet>>aChangeDePositionEtPasQueDeRotation;
                    if(aChangeDePositionEtPasQueDeRotation)
                    {
                        gestionBruits.jouerSon(IDautreJoueur, CHANGEMENT_POSITION);
                    }
                }
                break;

            case PACKET_MUNITION_CONSOMME:
                if(IDautreJoueur != monID)
                {
                    sf::Int32 numPaque;
                    core::vector3df position;
                    packet>>numPaque;
                    packet>>position;
                    paqueMunition[numPaque]->setPosition(position);
                    gestionBruits.jouerSon(IDautreJoueur, PACKET_MUNITION_CONSOMME);
                }
                break;

            case ARRETER_BOUGER:
                if(IDautreJoueur != monID)
                    gestionBruits.jouerSon(IDautreJoueur, ARRETER_BOUGER);
                break;

            case TIRE:
                if(IDautreJoueur != monID)
                {
                    gestionBruits.jouerSon(IDautreJoueur, TIRE);
                    listeToutsJoueurs[IDautreJoueur]->tire();
                }
                break;

            case EXPLOSION:
                if(IDautreJoueur != monID)
                {
                    core::vector3df position;
                    packet>>position;
                    gestionExplosion.addExplosion(position, IDautreJoueur);
                }
                break;

            case JOUEUR_ELIMINE:
                sf::Int32 idDuTueur;
                packet>>idDuTueur;
                myGui.joueurElimine(listeToutsJoueurs[idDuTueur]->getPseudo(), listeToutsJoueurs[IDautreJoueur]->getPseudo());
                if(listeClients[idDuTueur].equipe == monEquipe)
                {
                    receiver2->monEquipeGagneUnPoint();
                }

                else
                {
                    receiver2->adversaireGagneUnPoint();
                }
                break;
            }
        }

        //on envoie nos position dans cette fonction
        arreter = receiver2->miseAJour(driver->getScreenSize(),  joysticksInfo.size() > 0, FPS/10.00);

        //mise des position des pseudos des autres joueurs
        for(std::vector<Joueur*>::iterator it = listeAutreJoueurs.begin(); it != listeAutreJoueurs.end(); ++it)
        {
            (*it)->miseAJourPosTextPseudo(arreter);
        }

        gestionExplosion.miseAJour();

        //recherche manette
        tempsActuelle = timer->getTime();
        if(tempsActuelle > derniereRechercheDeManette + 1000)
        {
            derniereRechercheDeManette = tempsActuelle;
            device->activateJoysticks(joysticksInfo);
        }

        //FPS
        FPS = driver->getFPS();
        texteFps += FPS;
        texteFps += ")";
        device->setWindowCaption(texteFps.c_str());
        texteFps = "Guerre de tank - Client (FPS : ";

        driver->endScene();
    }

    for(unsigned int i = 0; i < listeToutsJoueurs.size(); ++i)
    {
        delete listeToutsJoueurs[i];
        listeToutsJoueurs[i] = 0;
    }

    delete receiver2;

    device->drop();

    return 0;
}
