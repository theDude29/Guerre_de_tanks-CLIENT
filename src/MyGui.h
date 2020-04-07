/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifndef GUI
#define GUI

#include <irrlicht/irrlicht.h>
#include "TankNormal.h"
#include "MyCamera.h"
#include "BoutonManette.h"
#include "GestionBruits.h"

enum TYPE_CONTROL {MANETTE, CLAVIER_SOURIS};
enum ACTION {AVANCER, RECULER, DROITE, GAUCHE};
enum TYPE_PARAMETRE {SON, GAMEPLAY, AIDEMANETTE, AUTRE};

class MyGui
{
public:
    MyGui(irr::IrrlichtDevice* device, irr::video::IVideoDriver* driver, irr::gui::IGUIEnvironment* gui, TankNormal* tank, MyCamera* camera);
    ~MyGui();
    void miseAJour(bool manetteDetecte, bool typeControl);
    void cacherToutsBouton();
    void montrerBoutonPrincipal();
    void cacherBoutonPrincipal();
    void montrerBoutonQuitter();
    void cacherBoutonQuitter();
    void fermerApplication();
    void montrerFenetreParametre();
    void cacherFenetreParametre();
    void montrerFenetreModificationPreference(ACTION action);
    void cacherFenetreModificationPreference();
    bool modificationTouche(const irr::SEvent::SKeyInput& newTouche, irr::EKEY_CODE keyAction[]);
    //on gere seulement les event du joystique qui on un rapport avec la gui (deplacer le curseur, voire si on clique sur un bouton...)
    void eventJoystick(const irr::SEvent::SJoystickEvent& m_joystickState,bool* dansLeMenuPrincipal, bool* dansLaFenetreQuitter, bool* dansLaFenetreOption, bool* typeControl);
    void changerTab(bool L1);
    void tabActuelleAChange(irr::s32 i);

    void setTextVolumeSonExplosion(irr::s32 volume);
    void setTextVolumeSonTire(irr::s32 volume);
    void setTextVolumeSonRechargement(irr::s32 volume);
    void setTextVolumeSonChenille(irr::s32 volume);
    void setTextVolumeSonImpossibleTirer(irr::s32 volume);
    void setTextNiveauSensibilite(irr::s32 sensibilite);

    void setGestionBruits(GestionBruits* gestionBruits);

    void joueurElimine(irr::core::stringw tueur, irr::core::stringw tue);

private:
    //partie importante de irrlicht
    irr::IrrlichtDevice* m_device;
    irr::video::IVideoDriver* m_driver;
    irr::gui::IGUIEnvironment* m_gui;
    //tout les bouton , scroll bar ...
    irr::gui::IGUIListBox* m_infoPartie;
    irr::gui::IGUIButton* m_quitter;
    irr::gui::IGUIButton* m_option;
    irr::gui::IGUIButton* m_annuler;

    irr::gui::IGUIWindow* m_verificationQuitter;

    irr::gui::IGUICheckBox* m_cbManetteDetecte;
    irr::gui::IGUIWindow* m_fenetreNouvelleTouche;
    irr::gui::IGUIStaticText* m_textNouvelleTouche;
    ACTION m_action;
    irr::gui::IGUIButton* m_BoutonNouvelleTouche[4];


    irr::gui::IGUIStaticText *m_textTire, *m_textRechargement, *m_textChenille, *m_textImpossibleDeTirer, *m_textSensibilite, *m_textExplosion;

    irr::gui::IGUITabControl* m_tabControl;
    irr::gui::IGUITab* m_tabSon;
    irr::gui::IGUITab* m_tabGameplay;
    irr::gui::IGUITab* m_tabAide;

    //element du jeu qui peuvent etre affecter par la gui
    TankNormal* m_tank;
    MyCamera* m_camera;

    //variable pour savoir ce qui est necessaire de mettre a jour
    bool m_boutonPrincipalVisible, m_boutonQuitterVisible, m_boutonParametreVisible,
      m_fenetreModificationPreferenceVisible;
    int m_TypeParametre;

    //autre;
    GestionBruits* m_gestionBruits;
    irr::video::ITexture* m_imageAideManette;
    BoutonManette* m_etatBoutonManette;
    irr::gui::ICursorControl* m_cursor;
    irr::core::dimension2d<irr::u32> m_screenSize;
};

#endif // GUI
