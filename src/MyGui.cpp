/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#include <irrlicht/irrlicht.h>
#include "MyGui.h"

using namespace irr;

MyGui::MyGui(irr::IrrlichtDevice* device,irr::video::IVideoDriver* driver, irr::gui::IGUIEnvironment* gui, TankNormal* tank, MyCamera* camera)
{
    //partie importante de irrlicht
    m_device = device; //on tourne  le corps a gauche
    m_driver = driver;
    m_gui = gui;

    //bouton...
    m_infoPartie = m_gui->addListBox(core::rect<s32>(0,0,180,250));
    m_infoPartie->addItem(L"La Partie a commencée !");

    m_quitter = m_gui->addButton(core::rect<s32>(-200,-50,0,0), 0, 1, L"QUITTER");
    m_quitter->setVisible(false);

    m_option = m_gui->addButton(core::rect<s32>(-200,-50,0,0), 0, 2, L"OPTION");
    m_option->setVisible(false);

    m_annuler = m_gui->addButton(core::rect<s32>(-200,-50,0,0),0, 0, L"ANNULER");
    m_annuler->setVisible(false);

    //

    m_tabControl = m_gui->addTabControl(core::rect<s32>(100, 50, 700, 480), 0, false, true, 40);
    m_tabControl->setVisible(false);

    //son
    m_tabSon = m_tabControl->addTab(L"Volume des sons");

    gui::IGUIScrollBar* sb;

    m_gui->addStaticText(L"Explosion :", core::rect<s32>(50,15,110,35), false, true, m_tabSon);
    m_textExplosion = m_gui->addStaticText(L"100", core::rect<s32>(110,15,140,35), false, true, m_tabSon);
    sb = m_gui->addScrollBar(true, core::rect<s32>(50,35,510,65), m_tabSon, 1500);
    sb->setPos(100);

    m_gui->addStaticText(L"Tire :", core::rect<s32>(50,80,80,100), false, true, m_tabSon);
    m_textTire = m_gui->addStaticText(L"50", core::rect<s32>(85,80,105,100), false, true, m_tabSon);
    sb = m_gui->addScrollBar(true, core::rect<s32>(50,100,510,130), m_tabSon, 11);
    sb->setPos(50);

    m_gui->addStaticText(L"Rechargement :", core::rect<s32>(50,145,130,165), false, true, m_tabSon);
    m_textRechargement = m_gui->addStaticText(L"80", core::rect<s32>(135,145,155,165), false, true, m_tabSon);
    sb = m_gui->addScrollBar(true, core::rect<s32>(50,165,510,195), m_tabSon, 12);
    sb->setPos(80);

    m_gui->addStaticText(L"Chenille :", core::rect<s32>(50,215,100,235), false, true, m_tabSon);
    m_textChenille = m_gui->addStaticText(L"3", core::rect<s32>(100,215,120,235), false, true, m_tabSon);
    sb = m_gui->addScrollBar(true, core::rect<s32>(50,235,510,265), m_tabSon, 13);
    sb->setPos(3);

    m_gui->addStaticText(L"Impossible de tirer :", core::rect<s32>(50,290,160,310), false, true, m_tabSon);
    m_textImpossibleDeTirer = m_gui->addStaticText(L"40", core::rect<s32>(160,290,180,310), false, true, m_tabSon);
    sb = m_gui->addScrollBar(true, core::rect<s32>(50,310,510,340), m_tabSon, 14);
    sb->setPos(40);

    //gameplay
    gui::IGUITab* m_tabGameplay = m_tabControl->addTab(L"Gameplay");

    m_gui->addStaticText(L"Sensibilité de la Camera :", core::rect<s32>(50,30,180,50), false, true, m_tabGameplay);
    m_textSensibilite = m_gui->addStaticText(L"30", core::rect<s32>(180,30,200,50), false, true, m_tabGameplay, 25);
    sb = m_gui->addScrollBar(true, core::rect<s32>(50, 50, 510, 80), m_tabGameplay, 15);
    sb->setPos(30);

    m_gui->addStaticText(L"Note : Le gameplay du jeu n'est pas adapté pour joué sans manette"
                         , core::rect<s32>(50, 120, 500, 140), false, true, m_tabGameplay);
    m_cbManetteDetecte = m_gui->addCheckBox(false, core::rect<s32>(50, 140, 600, 180), m_tabGameplay, 30);

    m_gui->addStaticText(L"Note : Les préférences sont paramètrable uniquement si vous jouez avec un clavier :\n", core::rect<s32>(50,200,500,220), false, true, m_tabGameplay);
    m_gui->addStaticText(L"Avancer :\n\nReculer :\n\nDroite :\n\nGauche :", core::rect<s32>(60,220,500,500), false, true, m_tabGameplay);

    m_BoutonNouvelleTouche[AVANCER] = m_gui->addButton(core::rect<s32>(130, 220, 200, 240), m_tabGameplay, 300, L"z", L"Cliquez pour changer\n la touche qui permet d'avancer");
    m_BoutonNouvelleTouche[RECULER] = m_gui->addButton(core::rect<s32>(130, 248, 200, 268), m_tabGameplay, 301, L"s", L"Cliquez pour changer\n la touche qui permet de reculer");
    m_BoutonNouvelleTouche[DROITE] = m_gui->addButton(core::rect<s32>(130, 278, 200, 298), m_tabGameplay, 302, L"d", L"Cliquez pour changer\n la touche qui permet de s'orienter à droite");
    m_BoutonNouvelleTouche[GAUCHE] = m_gui->addButton(core::rect<s32>(130, 308, 200, 328), m_tabGameplay, 303, L"q", L"Cliquez pour changer\n la touche qui permet de s'orienter à gauche");

    m_fenetreNouvelleTouche = m_gui->addWindow(core::rect<s32>(0, 0, 350, 170), true, L"Modification des préfèrences", 0, 51);
    m_fenetreNouvelleTouche->setVisible(false);
    m_fenetreNouvelleTouche->setDraggable(false);
    m_gui->addStaticText(L"Appuyez sur la touche\nque vous voulez affecter a cette action", core::rect<s32>(30, 30, 350, 100), false, true, m_fenetreNouvelleTouche);
    m_textNouvelleTouche = m_gui->addStaticText(L"", core::rect<s32>(40, 80, 350, 100), false, true, m_fenetreNouvelleTouche);
    m_gui->addButton(core::rect<s32>(10, 130, 100, 160), m_fenetreNouvelleTouche, 305, L"Annuler");

    //aide
    m_tabAide = m_tabControl->addTab(L"Aide manette");
    //

    m_verificationQuitter = m_gui->addWindow(core::rect<s32>(-300,-80,0,0), true, L"Voulez-vous vraiment quitter l'application ?", 0, 50);
    m_verificationQuitter->setDraggable(false);
    m_verificationQuitter->setVisible(false);

    m_gui->addButton(core::rect<s32>(180, 30, 260,60), m_verificationQuitter, 5, L"Confirmer");
    m_gui->addButton(core::rect<s32>(40, 30, 120,60), m_verificationQuitter, 6, L"Annuler");

    m_gui->addButton(core::rect<s32>(30, 390, 120, 420), m_tabControl, 10, L"Retour");


    //element du jeu qui peuvent etre affecter par la gui
    m_tank = tank;
    m_camera = camera;

    //variable pour savoir ce qui est necessaire de mettre a jour
    m_TypeParametre = AUTRE;
    m_boutonPrincipalVisible = false;
    m_boutonQuitterVisible = false;
    m_boutonParametreVisible = false;
    m_fenetreModificationPreferenceVisible = false;

    //autre
    m_etatBoutonManette = new BoutonManette();
    m_cursor = m_device->getCursorControl();
    m_screenSize = m_driver->getScreenSize();
}

void MyGui::miseAJour(bool manetteDetecte, bool typeControl)
{
    m_screenSize = m_driver->getScreenSize();

    if(manetteDetecte)
    {
        m_cbManetteDetecte->setText(L"Utilisation d'une manette de PS4 pour controler le tank\n(manette detectee)");
    }
    else
    {
        m_cbManetteDetecte->setText(L"Utilisation d'une manette de PS4 pour controler le tank\n(Aucune manette de PS4 détéctée vous ne pouvez donc pas utiliser les commandes par manette)");
        m_cbManetteDetecte->setChecked(false);
    }

    for(int i = 0; i < 4; ++i)
    {
        m_BoutonNouvelleTouche[i]->setEnabled(typeControl);
    }

    core::position2di positionElement;
    irr::s32 m_screenSize_middleWidth = m_screenSize.Width/2;
    irr::s32 m_screenSize_middleHeight = m_screenSize.Height/2;

    positionElement.X = 30;
    positionElement.Y = m_screenSize.Height - 280;
    m_infoPartie->setRelativePosition(positionElement);

    if(m_boutonPrincipalVisible)
    {
        positionElement.X = m_screenSize_middleWidth - 100;
        positionElement.Y = m_screenSize.Height/3;
        m_quitter->setRelativePosition(positionElement);

        positionElement.Y -= 80;
        m_option->setRelativePosition(positionElement);

        positionElement.Y += 160;
        m_annuler->setRelativePosition(positionElement);
    }

    if(m_boutonQuitterVisible)
    {
        positionElement.X = m_screenSize_middleWidth - 150;
        positionElement.Y = m_screenSize_middleHeight - 100;
        m_verificationQuitter->setRelativePosition(positionElement);
    }

    if(m_fenetreModificationPreferenceVisible)
    {
        positionElement.X = m_screenSize_middleWidth - 80;
        positionElement.Y = m_screenSize_middleHeight;
        m_fenetreNouvelleTouche->setRelativePosition(positionElement);
    }

    if(m_boutonParametreVisible)
    {
        positionElement.Y = m_screenSize_middleHeight - 250;
        positionElement.X = m_screenSize_middleWidth - 300;
        m_tabControl->setRelativePosition(positionElement);
        m_driver->draw2DRectangle(video::SColor(200,150,150,150), core::rect<s32>(positionElement.X, positionElement.Y, positionElement.X + 600, positionElement.Y + 430));
        //on met la tab control au premier plan
        m_tabControl->draw();
        m_fenetreNouvelleTouche->draw();
    }

    if(m_TypeParametre == AIDEMANETTE)
    {
        m_imageAideManette = m_driver->getTexture("texture/aide.png");
        positionElement.X = m_screenSize_middleWidth - 250;
        positionElement.Y = m_screenSize_middleHeight - 205;
        m_driver->draw2DImage(m_imageAideManette, positionElement);
    }
}

void MyGui::montrerBoutonPrincipal()
{
    m_quitter->setVisible(true);
    m_option->setVisible(true);
    m_annuler->setVisible(true);

    m_boutonPrincipalVisible = true;

    //on stop le son des chenilles au cas ou il ne serait pas stopper
//    m_tank->stopSonChenille();
}

void MyGui::cacherBoutonPrincipal()
{
    m_quitter->setVisible(false);
    m_option->setVisible(false);
    m_annuler->setVisible(false);

    m_boutonPrincipalVisible = false;
}

void MyGui::montrerBoutonQuitter()
{
   m_verificationQuitter->setVisible(true);

   m_boutonQuitterVisible = true;
}

void MyGui::cacherBoutonQuitter()
{
    m_verificationQuitter->setVisible(false);

    m_boutonQuitterVisible  = false;
}

void MyGui::fermerApplication()
{
    m_device->closeDevice();
}

void MyGui::montrerFenetreParametre()
{
    m_TypeParametre = m_tabControl->getActiveTab();
    m_tabControl->setVisible(true);
    m_boutonParametreVisible = true;
}

void MyGui::cacherFenetreParametre()
{
    m_tabControl->setVisible(false);
    m_TypeParametre = AUTRE;
    m_boutonParametreVisible = false;
}

void MyGui::montrerFenetreModificationPreference(ACTION action)
{
    m_textNouvelleTouche->setText(L"");
    m_action = action;
    m_fenetreNouvelleTouche->setVisible(true);
    m_fenetreModificationPreferenceVisible = true;
}

void MyGui::cacherFenetreModificationPreference()
{
    m_fenetreNouvelleTouche->setVisible(false);
    m_fenetreModificationPreferenceVisible = false;
}

bool MyGui::modificationTouche(const irr::SEvent::SKeyInput& newTouche, irr::EKEY_CODE keyAction[])
{
    irr::EKEY_CODE key = newTouche.Key;

    if(newTouche.Char == 0 || newTouche.Key == KEY_ESCAPE)
    {
        m_textNouvelleTouche->setText(L"Cette touche ne peut pas etre utilisée");
    }

    else
    if(key == keyAction[AVANCER] || key == keyAction[RECULER] || key == keyAction[DROITE] || key == keyAction[GAUCHE])
    {
        m_textNouvelleTouche->setText(L"Cette touche est déjà utilisé pour une autre action");
    }

    else
    {
        core::stringw text;
        text += newTouche.Char;
        text.erase(1);
        m_BoutonNouvelleTouche[m_action]->setText(text.c_str());
        keyAction[m_action] = key;
        cacherFenetreModificationPreference();
        return true;
    }

    return false;
}

void newPositionScrollBar(gui::IGUIScrollBar* sb, s32 posScrollBar, s32 posCursor)
{
    s32 etape = posCursor - (posScrollBar + 30);
    sb->setPos(etape/4);
}

void MyGui::eventJoystick(const irr::SEvent::SJoystickEvent& joystickState, bool* dansLeMenuPrincipal, bool* dansLaFenetreQuitter, bool* dansLaFenetreOption, bool* typeControl)
{
    //On met a jour les positions de la souris a intervalle regulier
    core::position2di posCursor = m_cursor->getPosition();
    s32 xCursor = posCursor.X;
    s32 yCursor = posCursor.Y;
    if(joystickState.Axis[SEvent::SJoystickEvent::AXIS_X] > 5000)
    {
        m_cursor->setPosition(xCursor + 5, yCursor);
    }

    if(joystickState.Axis[SEvent::SJoystickEvent::AXIS_X] < -5000)
    {
        m_cursor->setPosition(xCursor - 5, yCursor);
    }

    if(joystickState.Axis[SEvent::SJoystickEvent::AXIS_Y] > 5000)
    {
        m_cursor->setPosition(xCursor, yCursor + 5);
    }

    if(joystickState.Axis[SEvent::SJoystickEvent::AXIS_Y] < -5000)
    {
        m_cursor->setPosition(xCursor, yCursor - 5);
    }


    m_etatBoutonManette->miseAJour(joystickState);

    bool L1estCliquer = m_etatBoutonManette->L1_estClique();
    if(L1estCliquer || m_etatBoutonManette->R1_estClique())
    {
        changerTab(L1estCliquer);
    }

    if(m_etatBoutonManette->O_estClique()) //si on appui sur O on quitte le menu
    {
        if(*dansLeMenuPrincipal)
        {
            cacherBoutonPrincipal();
            *dansLeMenuPrincipal = false;
        }

        else
        if(*dansLaFenetreQuitter)
        {
            cacherBoutonQuitter();
            *dansLaFenetreQuitter = false;
            montrerBoutonPrincipal();
            *dansLeMenuPrincipal = true;
        }

        else
        {
            cacherFenetreParametre();
            *dansLaFenetreOption = false;
            montrerBoutonPrincipal();
            *dansLeMenuPrincipal = true;
        }
    }

    //si on appui sur X on regarde si on clique sur un bouton
    //et on declanche l action consequente
    if(m_etatBoutonManette->X_estClique())
    {
        gui::IGUIElement* element = m_gui->getRootGUIElement()->getElementFromPoint(m_cursor->getPosition());
        irr::s32 xElement = element->getAbsolutePosition().UpperLeftCorner.X;
        s32 id = element->getID();

        gui::IGUICheckBox* cb = (gui::IGUICheckBox*)(element);
        gui::IGUIScrollBar* sb = (gui::IGUIScrollBar*)(element);


        if(*dansLeMenuPrincipal)
        {
            switch(id) //reste a savoir lequele
            {
            case 0 :
                cacherBoutonPrincipal();
                *dansLeMenuPrincipal = false;
                break;
            case 1 :
                cacherBoutonPrincipal();
                montrerBoutonQuitter();
                *dansLaFenetreQuitter = true;
                *dansLeMenuPrincipal = false;
                break;
            case 2 :
                cacherBoutonPrincipal();
                montrerFenetreParametre();
                *dansLeMenuPrincipal = false;
                *dansLaFenetreOption = true;
                break;
            }
        }

        if(*dansLaFenetreQuitter)
        {
            switch(id)
            {
            case 5 :
                fermerApplication();
                break;
            case 6 :
                cacherBoutonQuitter();
                *dansLaFenetreQuitter = false;
                montrerBoutonPrincipal();
                *dansLeMenuPrincipal = true;
                break;
            }
        }

        if(*dansLaFenetreOption)
        {
            if(id == 10)
            {
                cacherFenetreParametre();
                *dansLaFenetreOption = false;
                montrerBoutonPrincipal();
                *dansLeMenuPrincipal = true;
            }

            if(m_TypeParametre == GAMEPLAY)
            {
                switch(id)
                {
                case 15:
                    newPositionScrollBar(sb, xElement, xCursor);
                    m_camera->setRotateSpeed(sb->getPos());
                    m_tank->setRotateSpeedCanon(sb->getPos());
                    setTextNiveauSensibilite(sb->getPos());
                    break;
                case 30:
                    if(xCursor < xElement + 20)
                    {
                        *typeControl = cb->isChecked();
                        cb->setChecked(!cb->isChecked());
                    }
                    break;
                }
            }

            else
            {
                gui::IGUIScrollBar* sb = (gui::IGUIScrollBar*)(element);

                switch(id)
                {
                case 11:
                    newPositionScrollBar(sb, xElement, xCursor);
                    m_gestionBruits->setVolumeTire(sb->getPos());
                    setTextVolumeSonTire(sb->getPos());
                    break;
                case 12:
                    newPositionScrollBar(sb, xElement, xCursor);
                    m_gestionBruits->setVolumeRechargement(sb->getPos());
                    setTextVolumeSonRechargement(sb->getPos());
                    break;
                case 13:
                    newPositionScrollBar(sb, xElement, xCursor);
                    m_gestionBruits->setVolumeChenille(sb->getPos());
                    setTextVolumeSonChenille(sb->getPos());
                    break;
                case 14:
                    newPositionScrollBar(sb, xElement, xCursor);
                    m_tank->setVolumeSonImpossibleTirer(sb->getPos());
                    setTextVolumeSonImpossibleTirer(sb->getPos());
                    break;
                }
            }
        }
    }
}

void MyGui::changerTab(bool L1)
{
    s32 i = m_tabControl->getActiveTab();

    if(L1) {i--;}
    else {i++;}

    if(i > 2) {i = 0;}
    if(i < 0) {i = 2;}

    m_tabControl->setActiveTab(i);
}

void MyGui::tabActuelleAChange(irr::s32 i)
{
    m_TypeParametre = i;
}

void MyGui::setTextVolumeSonTire(irr::s32 volume)
{
    core::stringw text = "";
    text += volume;
    m_textTire->setText(text.c_str());
}

void MyGui::setTextVolumeSonRechargement(irr::s32 volume)
{
    core::stringw text = "";
    text += volume;
    m_textRechargement->setText(text.c_str());
}

void MyGui::setTextVolumeSonChenille(irr::s32 volume)
{
    core::stringw text = "";
    text += volume;
    m_textChenille->setText(text.c_str());
}


void MyGui::setTextVolumeSonImpossibleTirer(irr::s32 volume)
{
    core::stringw text = "";
    text += volume;
    m_textImpossibleDeTirer->setText(text.c_str());
}

void MyGui::setTextNiveauSensibilite(irr::s32 sensibilite)
{
    core::stringw text = "";
    text += sensibilite;
    m_textSensibilite->setText(text.c_str());
}

void MyGui::setGestionBruits(GestionBruits* gestionBruits)
{
    m_gestionBruits = gestionBruits;
}

void MyGui::setTextVolumeSonExplosion(irr::s32 volume)
{
    core::stringw text = "";
    text += volume;
    m_textExplosion->setText(text.c_str());
}

void MyGui::joueurElimine(irr::core::stringw tueur, irr::core::stringw tue)
{
    core::stringw text = L"";
    text += tueur;
    text += L" a éliminé ";
    text += tue;
    m_infoPartie->addItem(text.c_str());
}

void MyGui::cacherToutsBouton()
{
    cacherBoutonPrincipal();
    cacherBoutonQuitter();
    cacherFenetreModificationPreference();
    cacherFenetreParametre();
    m_infoPartie->setVisible(false);
}

MyGui::~MyGui()
{
    delete m_etatBoutonManette;
}

