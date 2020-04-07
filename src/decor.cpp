/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#include <irrlicht/irrlicht.h>
#include <ctime>
#include <vector>

using namespace irr;

void creerDecor(scene::ISceneManager* sceneManager, video::IVideoDriver* driver, irr::scene::ITerrainSceneNode* terrain, irr::scene::IMeshSceneNode* paqueMunition[], std::vector<irr::core::vector3df> posPaqueMunition)
{
    //Terrain
    terrain->setScale(core::vector3df(50,2,50));
    //texture
    terrain->setMaterialTexture(0, driver->getTexture("texture/terrain-texture.jpg"));
    terrain->setMaterialTexture(1, driver->getTexture("texture/detailmap3.jpg"));
    terrain->scaleTexture(1,50);
    //materiau
    terrain->setMaterialType(video::EMT_DETAIL_MAP);

    //PAQUE DE MUNITION
    core::vector3df position;
    scene::IMesh* mesh = sceneManager->getMesh("mesh/paqueMunition.obj");
    irr::scene::ISceneNodeAnimator* anm;

    for(int i = 0 ; i < 10 ; ++i)
    {
        position.X = posPaqueMunition[i].X;
        position.Z = posPaqueMunition[i].Z;
        //la position en y est la hautuer du terrain au position x et z
        position.Y = terrain->getHeight(position.X, position.Z);

        //on creer le paquet a la position donnee
        paqueMunition[i] = sceneManager->addMeshSceneNode(mesh, 0, -1, position);
        //on lui met une texture
        paqueMunition[i]->setMaterialTexture(0, driver->getTexture("texture/texturePaqueMunition.jpg"));
        //on lui ajoute un animator de rotation
        anm = sceneManager->createRotationAnimator(core::vector3df(0,0.5,0));
        paqueMunition[i]->addAnimator(anm);
        anm->drop();
        //on le met un peu plus grand
        paqueMunition[i]->setScale(core::vector3df(80,80,80));
    }

    //Ciel
    sceneManager->addSkyDomeSceneNode(driver->getTexture("texture/skydome.jpg"));
}
