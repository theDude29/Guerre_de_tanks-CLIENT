/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifndef EXPLO
#define EXPLO

#include <irrlicht/irrlicht.h>
#include <SFML/Audio.hpp>

class Explosion
{
public:
    Explosion(irr::scene::ISceneManager* sceneManager, irr::ITimer* timer, irr::video::ITexture* animExplosion[], irr::core::vector3df posExplosion);
    bool miseAJour();

private:
    //partie importante irrlicht
    irr::ITimer* m_timer;
    irr::scene::ISceneManager* m_sceneManager;

    //autre
    irr::scene::IBillboardSceneNode* m_explosion;
    irr::video::ITexture** m_animExplosion;
    irr::u32 m_heureDerniereImage;
    irr::s32 m_indiceAnimation;
};

#endif // EXPLOSION
