#include "MyEventReceiver2.h"
#include <iostream>
#include <irrlicht/irrlicht.h>

using namespace irr;

MyEventReceiver2::MyEventReceiver2(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver* driver,irr::ITimer* timer, MyGui* myGui, TankNormal* tank, MyCamera* camera, irr::gui::ICursorControl* cursor, sf::IpAddress adresseServeur, unsigned short portServeur, sf::Int32 monID, GestionBruits* gestionBruits, sf::Int32 limiteRafraichissement, sf::Int32 nbEliminationGagnePartie, sf::UdpSocket* socket) : irr::IEventReceiver()
{
    //RESEAU
    m_adresseServeur = adresseServeur;
    m_portServeur = portServeur;
    m_monID = monID;
    m_socket= socket;

    //partie importante irrlicht
    m_sceneManager = sceneManager;
    m_driver = driver;
    m_timer = timer;

    //partie importante du jeu
    m_tank = tank;
    m_myGui = myGui;
    m_camera = camera;

    m_typeControl = CLAVIER_SOURIS;

    //CLAVIER
    m_keyAction[AVANCER] = irr::KEY_KEY_Z;
    m_keyAction[RECULER] = irr::KEY_KEY_S;
    m_keyAction[DROITE] = irr::KEY_KEY_D;
    m_keyAction[GAUCHE] = irr::KEY_KEY_Q;

    m_direction[AVANCER] = false;
    m_direction[RECULER] = false;
    m_direction[DROITE] = false;
    m_direction[GAUCHE] = false;

    //SOURIS
    m_buttonLeftPressed = false;
    m_buttonRightPressed = false;

    //pour se reperer dans la gui
    m_dansLeMenuPrincipale = false;
    m_dansLaFenetreQuitter = false;
    m_dansLaFenetreOption = false;
    m_dansLaFenetreModifPreference = false;

    //victoire?
    m_finDeLaPartie = false;
    m_quitter = false;

    m_bQuitter = m_sceneManager->getGUIEnvironment()->addButton(core::rect<s32>(0,0, 200, 50), 0, 10000, L"Quitter");
    m_bQuitter->setVisible(false);

    m_soundBufferVictoire.loadFromFile("son/victoire.wav");
    m_sonVictoire.setBuffer(m_soundBufferVictoire);
    m_sonVictoire.stop();

    m_imageGagne = m_driver->getTexture("texture/victoire.png");
    m_driver->makeColorKeyTexture(m_imageGagne, core::position2d<s32>(0,0));
    m_imagePerdu = m_driver->getTexture("texture/defaite.png");
    m_driver->makeColorKeyTexture(m_imagePerdu, core::position2d<s32>(0,0));

    m_nbEliminationGagnePartie = nbEliminationGagnePartie;
    m_nbEliminationAdversaire = 0;
    m_nbEliminationMonEquipe = 0;

    core::stringw text = L"Vous : ";
    text += m_nbEliminationMonEquipe;
    text += "/";
    text += m_nbEliminationGagnePartie;
    m_textInfoNbEliminationMonEquipe = m_sceneManager->getGUIEnvironment()->addStaticText(text.c_str(), core::rect<s32>(0,0,300, 50));

    text = L"Adversaires : ";
    text += m_nbEliminationAdversaire;
    text += "/";
    text += m_nbEliminationGagnePartie;
    m_textInfoNbEliminationAdversaire = m_sceneManager->getGUIEnvironment()->addStaticText(text.c_str(), core::rect<s32>(0,0,300, 50));

    //autre
    m_dernierRafraichissement = 0;
    m_limiteRafraichissement = limiteRafraichissement;
    m_gestionBruits = gestionBruits;
    m_onVientDeSaretter = false;
    m_viseur = driver->getTexture("texture/viseur.png");
    m_cursor = cursor;
}

bool MyEventReceiver2::OnEvent(const irr::SEvent& event)
{
    if(m_typeControl == MANETTE)
    {
        //si on recoi un evenement de la mannette on met a jour m_joystiqueState pour faire des testes apres
        if(event.EventType == irr::EET_JOYSTICK_INPUT_EVENT)
        {
            m_joystickState = event.JoystickEvent;
        }
    }

    if(m_typeControl == CLAVIER_SOURIS)
    {
        //evenement de la souris : on met a jour les varibles la concernant
        if(event.EventType == EET_MOUSE_INPUT_EVENT)
        {
            switch(event.MouseInput.Event)
            {
            case EMIE_LMOUSE_PRESSED_DOWN :
                m_buttonLeftPressed = true;
                break;
            case EMIE_LMOUSE_LEFT_UP :
                m_buttonLeftPressed = false;
                break;
            case EMIE_RMOUSE_PRESSED_DOWN :
                m_buttonRightPressed = true;
                break;
            case EMIE_RMOUSE_LEFT_UP :
                m_buttonRightPressed = false;
                break;
            case EMIE_MOUSE_MOVED :
                s32 milieuLargeurEcran, milieuHauteurEcran;
                milieuLargeurEcran = m_screenSize.Width/2;
                milieuHauteurEcran = m_screenSize.Height/2;

                m_mouvementSouris.X = event.MouseInput.X - milieuLargeurEcran;
                m_mouvementSouris.Y = event.MouseInput.Y - milieuHauteurEcran;
                break;

            default : break;
            }
        }

        if(event.EventType == EET_KEY_INPUT_EVENT)
        {
            bool presse = event.KeyInput.PressedDown;
            irr::EKEY_CODE key = event.KeyInput.Key;

            if(!m_dansLeMenuPrincipale && !m_dansLaFenetreOption && !m_dansLaFenetreQuitter)
            {
                if(key == KEY_ESCAPE)
                {
                    m_myGui->montrerBoutonPrincipal();
                    m_dansLeMenuPrincipale = true;
                    return true;
                }

                if(key == m_keyAction[AVANCER])
                {
                    m_direction[AVANCER] = presse;
                    return true;
                }

                else
                if(key == m_keyAction[RECULER])
                {
                    m_direction[RECULER] = presse;
                    return true;
                }

                else
                if(key == m_keyAction[GAUCHE])
                {
                    m_direction[GAUCHE] = presse;
                    return true;
                }

                else
                if(key == m_keyAction[DROITE])
                {
                    m_direction[DROITE] = presse;
                    return true;
                }

            }

            if(m_dansLaFenetreModifPreference)
            {
                if(m_myGui->modificationTouche(event.KeyInput, m_keyAction))
                {
                    m_dansLaFenetreModifPreference = false;
                }
            }

            else
                return false;
        }
    }

    if(event.EventType == EET_GUI_EVENT)
    {
        s32 id = event.GUIEvent.Caller->getID(); // on recupere l id

        //on recupere maintenant l elemen sous forme de scroll bar
        //car on ne peut pas le faire apres (WTF)
        //et on en aura besoin si l evenemtn venait effectivement d une scroll
        //bar (pour savoir la valeur de la scroll bar)
        gui::IGUIScrollBar* sb = (gui::IGUIScrollBar*)event.GUIEvent.Caller;
        switch(event.GUIEvent.EventType) // on anallise l appeler
        {
        //et on fait differente chose suivant l element ayant creer l evenement
        case gui::EGET_SCROLL_BAR_CHANGED :
            switch(id)
            {
            case 1500:
                m_gestionBruits->setVolumeExplosion(sb->getPos());
                m_myGui->setTextVolumeSonExplosion(sb->getPos());
                return true;
            case 11 :
                m_gestionBruits->setVolumeTire(sb->getPos());
                m_myGui->setTextVolumeSonTire(sb->getPos());
                return true;
            case 12 :
                m_gestionBruits->setVolumeRechargement(sb->getPos());
                m_myGui->setTextVolumeSonRechargement(sb->getPos());
                return true;
            case 13 :
                m_gestionBruits->setVolumeChenille(sb->getPos());
                m_myGui->setTextVolumeSonChenille(sb->getPos());
                return true;
            case 14 :
                m_tank->setVolumeSonImpossibleTirer(sb->getPos());
                m_myGui->setTextVolumeSonImpossibleTirer(sb->getPos());
                return true;
            case 15 :
                m_camera->setRotateSpeed(sb->getPos());
                m_tank->setRotateSpeedCanon(sb->getPos());
                m_myGui->setTextNiveauSensibilite(sb->getPos());
                return true;

            default : return false;
            }

        case gui::EGET_ELEMENT_CLOSED :
            if(id == 50)
            {
                m_myGui->cacherBoutonQuitter();
                m_dansLaFenetreQuitter = false;
                m_myGui->montrerBoutonPrincipal();
                m_dansLeMenuPrincipale = true;
                return true;
            }

            if(id == 51)
            {
                m_myGui->cacherFenetreModificationPreference();
                m_dansLaFenetreModifPreference = false;
                return true;
            }
            return false;

        case gui::EGET_BUTTON_CLICKED : //si bouton
            switch(id) // reste a savoir lequel
            {
            case 0 :
                m_myGui->cacherBoutonPrincipal();
                m_dansLeMenuPrincipale = false;
                return true;
            case 1 :
                m_myGui->cacherBoutonPrincipal();
                m_dansLeMenuPrincipale = false;
                m_myGui->montrerBoutonQuitter();
                m_dansLaFenetreQuitter = true;
                return true;
            case 2 :
                m_myGui->cacherBoutonPrincipal();
                m_dansLeMenuPrincipale = false;
                m_myGui->montrerFenetreParametre();
                m_dansLaFenetreOption = true;
                return true;
            case 5 :
                m_myGui->fermerApplication();
                return true;
            case 6 :
                m_myGui->cacherBoutonQuitter();
                m_dansLaFenetreQuitter = false;
                m_myGui->montrerBoutonPrincipal();
                m_dansLeMenuPrincipale = true;
                return true;
            case 10 :
                m_myGui->cacherFenetreParametre();
                m_dansLaFenetreOption = false;
                m_myGui->montrerBoutonPrincipal();
                m_dansLeMenuPrincipale = true;
                return true;
            case 300 :
                m_myGui->montrerFenetreModificationPreference(AVANCER);
                m_dansLaFenetreModifPreference = true;
                return true;
            case 301 :
                m_myGui->montrerFenetreModificationPreference(RECULER);
                 m_dansLaFenetreModifPreference = true;
                return true;
            case 302 :
                m_myGui->montrerFenetreModificationPreference(DROITE);
                 m_dansLaFenetreModifPreference = true;
                return true;
            case 303 :
                m_myGui->montrerFenetreModificationPreference(GAUCHE);
                 m_dansLaFenetreModifPreference = true;
                return true;
            case 305 :
                m_myGui->cacherFenetreModificationPreference();
                 m_dansLaFenetreModifPreference = false;
                return true;
            case 10000 :
                m_quitter = true;
                return true;

            default : return false;
            }

        case gui::EGET_CHECKBOX_CHANGED :
            if(id == 30)
            {
                if(m_typeControl == CLAVIER_SOURIS)
                    m_typeControl = MANETTE;
                else
                    m_typeControl = CLAVIER_SOURIS;
                return true;
            }
            return false;

        case gui::EGET_TAB_CHANGED :
                if(id == 40)
                {
                    gui::IGUITabControl* tab = (gui::IGUITabControl*)event.GUIEvent.Caller;
                    m_myGui->tabActuelleAChange(tab->getActiveTab());
                    return true;
                }
                return false;

        default : return false;
        }
    }
    //si ce n est aucune des sortes  evenement ci dessus on return false car on a pas utiliser l evenement
    return false;
}

bool MyEventReceiver2::miseAJour(irr::core::dimension2d<irr::u32> screenSize, bool manetteDetecte, irr::f32 coefficientFPS)
{
    //on met a jour la taille de l'ecran (on utilise une variable car on en a aussi
    //besoin dans la fonction OnEvent)
    m_screenSize = screenSize;

    if(m_finDeLaPartie)
    {
        m_bQuitter->setRelativePosition(core::position2di(m_screenSize.Width/2 - 100, m_screenSize.Height/2 + 50));
        m_driver->draw2DImage(m_imageResultat, core::position2di(m_screenSize.Width/2 - m_tailleImage.Width/2, m_screenSize.Height/2 - 100), core::rect<s32>(0,0,m_tailleImage.Width, m_tailleImage.Height), 0, video::SColor(255,255,255,255), true);
    }

    else
    {
        if(!manetteDetecte)
            m_typeControl = CLAVIER_SOURIS;

        m_tank->adapterVitesseAuFps(coefficientFPS);

        m_driver->draw2DRectangle(video::SColor(230,0,0,255), core::rect<s32>(m_screenSize.Width / 2 - 150, 0, m_screenSize.Width / 2, 40));
        m_driver->draw2DRectangle(video::SColor(230,255,0,0), core::rect<s32>(m_screenSize.Width / 2, 0, m_screenSize.Width / 2 + 150, 40));

        core::position2di pos;
        pos.Y = 10;
        pos.X = m_screenSize.Width / 2 - 130;
        m_textInfoNbEliminationMonEquipe->setRelativePosition(pos);
        m_textInfoNbEliminationMonEquipe->draw();

        pos.X = m_screenSize.Width / 2 + 20;
        m_textInfoNbEliminationAdversaire->setRelativePosition(pos);
        m_textInfoNbEliminationAdversaire->draw();

        if(!m_dansLeMenuPrincipale && !m_dansLaFenetreQuitter && !m_dansLaFenetreOption)
        {
            //pour que le curseur ne nous gene pas pendant le jeu
            m_cursor->setVisible(false);

            //VISEUR
            m_driver->draw2DImage(m_viseur, core::dimension2d<s32>(m_screenSize.Width/2 - 40, m_screenSize.Height/2 - 40), core::rect<s32>(0,0,80,80), 0,  video::SColor(255,255,255,255), true);

            if(m_typeControl == CLAVIER_SOURIS)//On gere les mouvement du tank avec les touche de clavier et la souris
            {
                if(m_direction[AVANCER])
                {
                    m_tank->avancer(false);
                }

                else
                if(m_direction[RECULER])
                {
                    m_tank->avancer(true);
                }

                else
                {
                    m_gestionBruits->jouerSon(m_monID, ARRETER_BOUGER);
                }

                if(m_direction[GAUCHE])
                    m_tank->tournerCorps(true);
                if(m_direction[DROITE])
                    m_tank->tournerCorps(false);

                if(!m_buttonRightPressed)
                {
                    if(m_mouvementSouris.X != 0 || m_mouvementSouris.Y != 0)
                    {
                        if(m_mouvementSouris.X < 0)
                        {
                            m_tank->tournerCanon(true);
                            m_camera->tournerHorizontal(true);
                            m_mouvementSouris.X = 0;
                        }

                        if(m_mouvementSouris.X > 0)
                        {
                            m_tank->tournerCanon(false);
                            m_camera->tournerHorizontal(false);
                            m_mouvementSouris.X = 0;
                        }

                        if(m_mouvementSouris.Y < 0)
                        {
                            m_camera->tournerVertical(true);
                            m_mouvementSouris.Y = 0;
                        }

                        if(m_mouvementSouris.Y > 0)
                        {
                            m_camera->tournerVertical(false);
                            m_mouvementSouris.Y = 0;
                        }

                        //on remet le curseur au milieu pour eviter qu il sorte de la fenetre
                        m_cursor->setPosition(0.5f, 0.5f);
                    }
                }

                if(m_buttonLeftPressed)
                {
                    if(m_tank->essaiTire())
                    {
                        m_tank->tire();
                    }
                }
            }


            if(m_typeControl == MANETTE)
            {
                if(m_joystickState.IsButtonPressed(7))
                {
                    //on regarde si on peut
                    if(m_tank->essaiTire())
                    {
                        //si on peut on tire
                        m_tank->tire();
                    }
                }

                //on tourne  le corps a gauche
                if(m_joystickState.IsButtonPressed(4))
                {
                    m_tank->tournerCorps(true);
                }

                //a droite
                if(m_joystickState.IsButtonPressed(5))
                {
                    m_tank->tournerCorps(false);
                }

                //deplecement de tank dans la direction du corps
                //si on dirige le joystique dans n importe quelle directioon
                if(m_joystickState.Axis[SEvent::SJoystickEvent::AXIS_Y] < -0
                   || m_joystickState.Axis[SEvent::SJoystickEvent::AXIS_X] < -0
                   || m_joystickState.Axis[SEvent::SJoystickEvent::AXIS_Y] > 0
                   || m_joystickState.Axis[SEvent::SJoystickEvent::AXIS_X] > 0)
                {
                    m_tank->avancer(m_joystickState.IsButtonPressed(6));
                }

                //si ne bouge pas on arrette le son des chenille
                else
                {
                    m_gestionBruits->jouerSon(m_monID, ARRETER_BOUGER);
                }


                if(m_joystickState.Axis[SEvent::SJoystickEvent::AXIS_Z] < -10000)
                {
                    m_camera->tournerHorizontal(true);
                    m_tank->tournerCanon(true);
                }

                if(m_joystickState.Axis[SEvent::SJoystickEvent::AXIS_Z] > 10000)
                {
                    m_camera->tournerHorizontal(false);
                    m_tank->tournerCanon(false);
                }

                if(m_joystickState.Axis[SEvent::SJoystickEvent::AXIS_V] < -20000)
                {
                    m_camera->tournerVertical(true);
                }

                if(m_joystickState.Axis[SEvent::SJoystickEvent::AXIS_V] > 20000)
                {
                    m_camera->tournerVertical(false);
                }

                //Si on appui sur option on ouvre le menu
                if(m_joystickState.IsButtonPressed(9))
                {
                    m_myGui->montrerBoutonPrincipal();
                    m_dansLeMenuPrincipale = true;
                }
            }

            //on met a jour les position et rotation de la camera grace changement effectuer plus haut (tournervertical, horizontal..)
            //que ce soit avec la mantte ou les touche de clavier
            m_camera->miseAJour(m_tank->getCorps()->getPosition());

            //on adapte le tank au changement du terrain
            m_tank->ajusterPositionRotation();

            //On verifie si on est sur un paque de munition
            m_tank->rechargementMunition();

            m_tank->majShoots();
        }

        else //si on est dans un menu on gere les evement de la manette avec le myGui
        {
            m_cursor->setVisible(true);

            if(m_typeControl == MANETTE)
            {
                m_myGui->eventJoystick(m_joystickState, &m_dansLeMenuPrincipale, &m_dansLaFenetreQuitter, &m_dansLaFenetreOption, &m_typeControl);
            }
        }

        //ON MET LES ELEMENTS DE LA GUI A LEUR BONNE PLACE
        m_myGui->miseAJour(manetteDetecte, m_typeControl);

        //MISE A JOUR E L ANIMAITION DE RECHARGEMENT
        m_tank->majRechargementMunitionEtSante();

        //ENVOID DES POS AU SERVEUR
        infoPositionRotationJoueur info;
        info.pos = m_tank->getCorps()->getPosition();
        info.rotCorps = m_tank->getRotCorps();
        info.rotCanon = m_tank->getRotCanon();

        sf::Packet packet;

        //

        if(m_dernierRafraichissement < m_timer->getTime() - m_limiteRafraichissement)
        {
            m_dernierRafraichissement = m_timer->getTime();
            if(info != m_ancienneInfo)
            {
                bool onAChangeDePositionEtPasQueDeRotation = false;
                if(info.pos != m_ancienneInfo.pos)
                {
                    onAChangeDePositionEtPasQueDeRotation = true;
                }
                m_ancienneInfo = info;
                packet<<m_monID<<CHANGEMENT_POSITION_ROTATION<<info;
                packet<<onAChangeDePositionEtPasQueDeRotation;
                m_socket->send(packet, m_adresseServeur, m_portServeur);
                m_onVientDeSaretter = false;
            }

            else
            {
                if(!m_onVientDeSaretter)
                {
                    sf::Packet packet;
                    packet<<m_monID;
                    packet<<ARRETER_BOUGER;
                    m_socket->send(packet, m_adresseServeur, m_portServeur);
                    std::cout<<m_socket->getLocalPort()<<"\n";
                    m_onVientDeSaretter = true;
                }
            }
        }
    }

    if(m_quitter)
        m_bQuitter->setVisible(false);

    return m_quitter;
}

void MyEventReceiver2::monEquipeGagneUnPoint()
{
    m_nbEliminationMonEquipe++;

    core::stringw text = L"Vous : ";
    text += m_nbEliminationMonEquipe;
    text += "/";
    text += m_nbEliminationGagnePartie;
    m_textInfoNbEliminationMonEquipe->setText(text.c_str());

    if(m_nbEliminationMonEquipe == m_nbEliminationGagnePartie)
    {
        sf::Packet packet;
        m_socket->send(packet, m_adresseServeur, m_portServeur);

        m_finDeLaPartie = true;
        m_imageResultat = m_imageGagne;
        m_tailleImage.Width = 370;
        m_tailleImage.Height = 125;

        m_bQuitter->setVisible(true);
        m_sonVictoire.play();
        m_tank->cacherTextMunition();
        m_textInfoNbEliminationAdversaire->setVisible(false);
        m_textInfoNbEliminationMonEquipe->setVisible(false);
        m_myGui->cacherToutsBouton();
        m_cursor->setVisible(true);
    }
}

void MyEventReceiver2::adversaireGagneUnPoint()
{
    m_nbEliminationAdversaire++;

    core::stringw text = L"Adversaires : ";
    text += m_nbEliminationAdversaire;
    text += "/";
    text += m_nbEliminationGagnePartie;
    m_textInfoNbEliminationAdversaire->setText(text.c_str());

    if(m_nbEliminationAdversaire == m_nbEliminationGagnePartie)
    {
        m_finDeLaPartie = true;
        m_imageResultat = m_imagePerdu;
        m_tailleImage.Width = 300;
        m_tailleImage.Height = 105;

        m_bQuitter->setVisible(true);
        m_tank->cacherTextMunition();
        m_textInfoNbEliminationAdversaire->setVisible(false);
        m_textInfoNbEliminationMonEquipe->setVisible(false);
        m_myGui->cacherToutsBouton();
        m_cursor->setVisible(true);
    }
}
