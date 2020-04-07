/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#include <irrlicht/irrlicht.h>
#include "Explosion.h"
#include "vector3d_operateur.h"

using namespace irr;

Explosion::Explosion(irr::scene::ISceneManager* sceneManager,irr::ITimer* timer, irr::video::ITexture* animExplosion[], irr::core::vector3df posExplosion)
{
    //partie importante irrlicht
    m_timer = timer;
    m_sceneManager = sceneManager;

    //autre
    m_indiceAnimation = 0;
    m_heureDerniereImage = m_timer->getTime();
    m_animExplosion = animExplosion;

    m_explosion = m_sceneManager->addBillboardSceneNode(0, core::dimension2d<f32>(800,800));
    m_explosion->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    m_explosion->setMaterialTexture(0, m_animExplosion[0]);
    m_explosion->setPosition(posExplosion);
}

bool Explosion::miseAJour()
{
    if(m_timer->getTime() > m_heureDerniereImage + 40)
    {
        m_explosion->setMaterialTexture(0, m_animExplosion[m_indiceAnimation]);
        m_indiceAnimation += (m_timer->getTime() - m_heureDerniereImage) / 40;
        m_heureDerniereImage = m_timer->getTime();
        if(m_indiceAnimation > 59)
        {
            return true;
        }
    }

    return false;
}
