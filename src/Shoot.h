/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifndef SHOOT
#define SHOOT

#include <irrlicht/irrlicht.h>
#include "GestionExplosion.h"

class Shoot
{
public:
    Shoot(irr::scene::ISceneManager* sceneManager, GestionExplosion* gestionExplosion, irr::ITimer* timer, irr::scene::ISceneCollisionManager* collisionManager, irr::scene::ITriangleSelector* terrainSelector, irr::s32 vitesse, irr::core::vector3df direction, irr::core::vector3df depart, std::vector<Joueur*> listeJoueur);
    bool miseAJour();
    irr::core::vector3df getPosition();

private:
    //partie importantes irrlicht
    irr::ITimer* m_timer;
    irr::scene::ISceneCollisionManager* m_collisionManager;

    //variable pour gerer le shoot
    irr::f32 m_vitesse;
    int m_indiceDistance, m_maxIndiceDistance;
    irr::core::vector3df m_direction, m_point1, m_point2;

    //collision
    irr::scene::IMetaTriangleSelector* m_selector;

    //autre
    GestionExplosion* m_gestionExplosion;
};
#endif // SHOOT
