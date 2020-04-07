/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifndef GESTIONEXPLO
#define GESTIONEXPLO

#include <irrlicht/irrlicht.h>
#include <vector>
#include "Explosion.h"
#include <SFML/Network.hpp>
#include "GestionBruits.h"

class TankNormal;

class GestionExplosion
{
public:
    GestionExplosion(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver* driver, irr::ITimer* timer, sf::IpAddress adresseServeur, unsigned short portServeur, sf::Int32 monID);
    void addExplosion(irr::core::vector3df posExplo, sf::Int32 idDuTireur);
    void miseAJour();

    void setGestionBruits(GestionBruits* gestionBruits);
    void setMonJoueur(TankNormal* monJoueur);

private:
    //reseau
    sf::Int32 m_monID;
    sf::UdpSocket m_socket;
    sf::IpAddress m_adresseServeur;
    unsigned short m_portServeur;

    //partie importante irrlicht
    irr::video::IVideoDriver* m_driver;
    irr::scene::ISceneManager* m_sceneManager;
    irr::ITimer* m_timer;

    //autre
    TankNormal* m_monJoueur;
    GestionBruits* m_gestionBruits;
    std::vector<Explosion> m_listeExplosions;
    irr::video::ITexture* m_animExplo[60];
};

#endif // GESTIONEXPLO
