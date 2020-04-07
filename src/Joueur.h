/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifndef JOUEUR
#define JOUEUR

#include <irrlicht/irrlicht.h>
#include "infoPositionRotationJoueur.h"
#include <SFML/System/String.hpp>
#include "client.h"

class Joueur
{
public:
    Joueur(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver* driver, Client client, bool monJoueur, irr::scene::ITerrainSceneNode* terrain);
    virtual ~Joueur();
    void miseAJour(infoPositionRotationJoueur info);
    void miseAJourPosTextPseudo(bool finDeLaPartie);
    void tire();

    irr::scene::IMeshSceneNode* getSceneNode();
    irr::core::vector3df getPosition();
    irr::core::stringw getPseudo();

protected:
    //partie importante irrlicht
    irr::scene::ISceneManager* m_sceneManager;
    irr::video::IVideoDriver* m_driver;
    irr::scene::ISceneCollisionManager* m_collisionManager;

    //collision
    irr::scene::ITerrainSceneNode* m_terrain;
    irr::scene::ITriangleSelector* m_terrainSelector;

    //partie du tank
    irr::scene::IMeshSceneNode* m_corps;
    irr::scene::IMeshSceneNode* m_canon;

    //text du pseudo
    sf::String m_pseudo;
    irr::gui::IGUIStaticText* m_textPseudo;
    irr::s32 m_decalageDuText;

    //autre
    irr::scene::ICameraSceneNode* m_camera;
    sf::Uint32 m_id;
};

#endif // JOUEUR
