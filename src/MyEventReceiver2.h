#ifndef RECEIVER2
#define RECEIVER2

#include <irrlicht/irrlicht.h>
#include "TankNormal.h"
#include "MyGui.h"
#include "MyCamera.h"
#include "infoPositionRotationJoueur.h"

class MyEventReceiver2 : public irr::IEventReceiver
{
public :
    MyEventReceiver2(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver* driver, irr::ITimer* timer, MyGui* gui, TankNormal* tank, MyCamera* camera, irr::gui::ICursorControl* cursor, sf::IpAddress adresseServeur, unsigned short portServeur, sf::Int32 monID, GestionBruits* gestionBruits, sf::Int32 limiteRafraichissement, sf::Int32 nbEliminationGagnePartie, sf::UdpSocket* socket);
    virtual bool OnEvent(const irr::SEvent& event);
    bool miseAJour(irr::core::dimension2d<irr::u32> screenSize, bool manetteDetecte, irr::f32 coefficientFPS);
    void monEquipeGagneUnPoint();
    void adversaireGagneUnPoint();

private :
    //RESEAU
    sf::UdpSocket* m_socket;
    sf::IpAddress m_adresseServeur;
    unsigned short m_portServeur;
    infoPositionRotationJoueur m_ancienneInfo;
    sf::Int32 m_monID;

    //partie importante de irrlicht
    irr::scene::ISceneManager* m_sceneManager;
    irr::video::IVideoDriver* m_driver;
    irr::ITimer* m_timer;

    //myGui
    MyGui* m_myGui;
    //tank
    TankNormal* m_tank;
    //camera
    MyCamera* m_camera;

    bool m_typeControl;
    //SOURIS
    irr::u32 m_derniereMajPosSouris;
    irr::gui::ICursorControl* m_cursor;
    irr::core::vector2df m_mouvementSouris;
    bool m_buttonLeftPressed, m_buttonRightPressed;

    //MANETTE
    irr::SEvent::SJoystickEvent m_joystickState;

    //CLAVIER
    bool m_direction[4];
    irr::EKEY_CODE m_keyAction[4];

    //variable pour se situer dans la gui
    bool m_dansLeMenuPrincipale, m_dansLaFenetreQuitter, m_dansLaFenetreOption, m_dansLaFenetreModifPreference;

    //limite le rafraichissement
    irr::u32 m_dernierRafraichissement;
    sf::Int32 m_limiteRafraichissement;

    //victoire?
    bool m_quitter, m_finDeLaPartie;
    irr::gui::IGUIButton* m_bQuitter;
    sf::SoundBuffer m_soundBufferVictoire;
    sf::Sound m_sonVictoire;
    irr::video::ITexture* m_imageGagne, *m_imagePerdu, *m_imageResultat;
    irr::core::dimension2d<irr::u32> m_tailleImage;
    sf::Int32 m_nbEliminationGagnePartie, m_nbEliminationMonEquipe, m_nbEliminationAdversaire;
    irr::gui::IGUIStaticText* m_textInfoNbEliminationMonEquipe, *m_textInfoNbEliminationAdversaire;

    //autre
    GestionBruits* m_gestionBruits;
    bool m_onVientDeSaretter;
    irr::core::dimension2di m_screenSize;
    irr::video::ITexture* m_viseur;
};

#endif // CACA

