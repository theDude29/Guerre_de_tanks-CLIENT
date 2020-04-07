/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifndef RECEIVER
#define RECEIVER

#include <irrlicht/irrlicht.h>
#include <SFML/Network.hpp>
#include "client.h"
#include "vector3d_operateur.h"

class MyEventReceiver : public irr::IEventReceiver
{
public:
    MyEventReceiver(irr::gui::IGUIEnvironment* gui, irr::ITimer* timer);
    virtual bool OnEvent(const irr::SEvent& event);
    bool miseAJour(irr::core::dimension2d<irr::u32> screenSize);
    void tentativeDeConnexion();
    void miseAJourEtatDesConnection();
    bool laPartieCommence();

    unsigned short getPort();
    sf::Uint32 getID();
    sf::Uint32 getNbJoueursConnecte();
    unsigned short getPortServeur();
    sf::IpAddress getAdresseServeur();
    std::vector<Client> getListeClients();
    std::vector<irr::core::vector3df> getPosPaquesMunition();
    sf::Int32 getIntervalleRafraichissement();
    sf::Int32 getNbEliminationGagnePartie();

private:
    //partie importantes irrlicht
    irr::ITimer* m_timer;

    //reseau
    sf::TcpSocket m_socket;

    //info partie
    std::vector<irr::core::vector3df> m_posPaquesMunition;
    std::vector<Client> m_listeClients;
    sf::Int32 m_nbJoueursConnecte;
    sf::Int32 m_id;
    sf::Int32 m_intervalleRafraichissement;
    sf::Int32 m_nbEliminationGagnePartie;

    //gui
    irr::gui::IGUIEditBox *m_ebAdresseIP, *m_ebPort, *m_ebPseudo;
    irr::gui::IGUIButton* m_bRejoindre;
    irr::gui::IGUIStaticText *m_textPresentation, *m_textVersion, *m_textInfoConnectionServeur;

    //anim (.,..,...)
    irr::core::stringw m_animAttendreConnection[4];
    irr::u32 m_heureDerniereRafraichissementAnimAttendreConnection;
    irr::s8 m_indiceAnimAttendreConnection;

    //autre
    irr::u32 m_heureDernierAppuiTouchePrisEnCompte;
    irr::core::stringw m_textInfoConnexionServeur_contenu;
    bool m_ebAdresseIPFocused, m_ebPortFocused, m_connecte, m_enAttenteDesAutreJoueurs, m_enAttenteDeLaCreationDesEquipes;
};

#endif // RECEIVER
