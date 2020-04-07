/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifndef TANK
#define TANK

#include <list>
#include <irrlicht/irrlicht.h>
#include "vector3d_operateur.h"
#include "Joueur.h"
#include "Shoot.h"

class GestionBruits;

class TankNormal : public Joueur
{
public:
    TankNormal(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver* driver, irr::gui::IGUIEnvironment* gui, irr::ITimer* timer, irr::scene::ITerrainSceneNode* terrain, irr::scene::IMeshSceneNode** paqueMunition, Client client, sf::IpAddress adresseServeur, unsigned short portServeur, GestionExplosion* gestionExplosion);
    virtual ~TankNormal();

    void avancer(bool reculer);
    void tournerCanon(bool gauche);
    void tournerCorps(bool gauche);
    void ajusterPositionRotation();
    bool essaiTire();
    void tire();
    void rechargementMunition();
    void majRechargementMunitionEtSante();
    void majShoots();
    bool perdreDeLaSante();

    bool* estEnRechargement();
    irr::scene::IMeshSceneNode* getCorps() const;
    irr::core::vector3df getRotCorps() const;
    irr::core::vector3df getRotCanon() const;

    void setRotateSpeedCanon(irr::f32 rotateSpeed);
    void setVolumeSonImpossibleTirer(irr::s32 volume);

    void adapterVitesseAuFps(irr::f32 coefficientFPS);

    void setListeAutreJoueur(std::vector<Joueur*> listeAutreJoueurs);
    void setGestionBruits(GestionBruits* gestionBruits);

    void cacherTextMunition();

private:
    //RESEAU
    sf::UdpSocket m_socket;
    sf::IpAddress m_adresseServeur;
    unsigned short m_portServeur;
    sf::Int32 m_monID;

    //partie importante de irrlicht
    irr::ITimer* m_timer;
    irr::gui::IGUIEnvironment* m_gui;
    //ressource (son, image2d)
    irr::video::ITexture* m_imageStatutChargement;
    irr::video::ITexture*  m_imageStatutChargementAucuneMunition;
    irr::video::ITexture *m_barreSante, *m_barreDeSanteGrise;
    sf::SoundBuffer m_soundBufferSonImpossible;
    sf::Sound m_sonImpossible;
    //variable lie au statistique du tank (vitesse, temps de rechargement...)
    irr::s32 m_vitesse, m_rotateSpeedCanon, m_rotateSpeedCorps, m_vitesseObus,
    m_nombreDeMunition, m_nombreMaxDeMunition, m_dernierTire, m_tempsDeRechargement, m_nbDeVie;
    irr::core::vector3df m_positionDeDepart;

    //mort?
    bool m_onEstMort;
    irr::u32 m_heureDuDeces;

    //autre
    GestionExplosion* m_gestionExplosion;
    std::vector<Shoot> m_listeShoots;
    GestionBruits* m_gestionBruits;
    std::vector<Joueur*> m_listeAutreJoueurs;
    irr::scene::IMeshSceneNode** m_paqueMunition;
    irr::gui::IGUIStaticText* m_texteMunition;
    irr::core::dimension2d<irr::u32> m_screenSize;
};

#endif // TANK

