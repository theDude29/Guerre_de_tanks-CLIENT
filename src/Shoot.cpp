/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#include "Shoot.h"

using namespace irr;

Shoot::Shoot(irr::scene::ISceneManager* sceneManager, GestionExplosion* gestionExplosion, irr::ITimer* timer, irr::scene::ISceneCollisionManager* collisionManager, irr::scene::ITriangleSelector* terrainSelector, irr::s32 vitesse, irr::core::vector3df direction, irr::core::vector3df depart, std::vector<Joueur*> listeJoueur)
{
    //partie importantes irrlicht
    m_timer = timer;
    m_collisionManager = collisionManager;

    //variable pour gerer le shoot
    m_vitesse = vitesse;
    m_indiceDistance = 0;
    m_maxIndiceDistance = 10000/vitesse;
    m_direction = direction * vitesse;
    m_point1 = depart;
    m_point1.Y += 150;
    m_point2 = m_point1 + direction;

    //collision
    m_selector = sceneManager->createMetaTriangleSelector();
    m_selector->addTriangleSelector(terrainSelector);
    for(std::vector<Joueur*>::iterator it = listeJoueur.begin(); it != listeJoueur.end(); it++)
    {
        m_selector->addTriangleSelector(sceneManager->createTriangleSelector((*it)->getSceneNode()->getMesh(), (*it)->getSceneNode()));
    }

    //autre
    m_gestionExplosion = gestionExplosion;
}

bool Shoot::miseAJour()
{
    core::vector3df point;
    core::triangle3df triangle;
    scene::ISceneNode* node;

    core::line3df line;
    line.start = m_point1;
    line.end = m_point2;

    if(m_indiceDistance == m_maxIndiceDistance)
    {
        m_gestionExplosion->addExplosion(m_point2, -1);
        return true;
    }


    else
    if(m_collisionManager->getCollisionPoint(line, m_selector, point, triangle, node))
    {
        m_gestionExplosion->addExplosion(m_point2, -1);
        return true;
    }

    m_point1 = m_point2;
    m_point2 += m_direction;
    m_indiceDistance++;
    return false;
}

irr::core::vector3df Shoot::getPosition()
{
    return m_point2;
}
