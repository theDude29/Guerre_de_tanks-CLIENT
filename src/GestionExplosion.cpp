/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#include "GestionExplosion.h"
#include "client.h"
#include "vector3d_operateur.h"
#include "TankNormal.h"

using namespace irr;

GestionExplosion::GestionExplosion(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver* driver, irr::ITimer* timer, sf::IpAddress adresseServeur, unsigned short portServeur, sf::Int32 monID)
{
    m_driver = driver;
    m_sceneManager = sceneManager;
    m_timer = timer;

    m_adresseServeur = adresseServeur;
    m_portServeur = portServeur;
    m_monID = monID;

    //explosion
    for(int i = 0 ; i<60 ; i++)
    {
        core::stringc chemin = "texture/explosion/Xplosion";
        chemin += i+1;
        chemin += ".jpg";
        m_animExplo[i] = m_driver->getTexture(chemin);
    }
}

void GestionExplosion::addExplosion(core::vector3df posExplo, sf::Int32 idDuTireur)
{
    core::aabbox3df bbMonJoueur = m_monJoueur->getSceneNode()->getTransformedBoundingBox();
    if(bbMonJoueur.isPointInside(posExplo))
    {
        if(m_monJoueur->perdreDeLaSante())
        {
            sf::Packet packet;
            packet<<m_monID<<JOUEUR_ELIMINE<<idDuTireur;
            m_socket.send(packet, m_adresseServeur, m_portServeur);
        }
    }

    m_gestionBruits->jouerSonExplosion(posExplo);
    Explosion newExplosion(m_sceneManager, m_timer, m_animExplo, posExplo);
    m_listeExplosions.push_back(newExplosion);
}

void GestionExplosion::miseAJour()
{
    for(std::vector<Explosion>::iterator it = m_listeExplosions.begin(); it != m_listeExplosions.end();)
    {
        if((*it).miseAJour())
        {
            it = m_listeExplosions.erase(it);
        }

        else
        {
            ++it;
        }
    }
}

void GestionExplosion::setGestionBruits(GestionBruits* gestionBruits)
{
    m_gestionBruits = gestionBruits;
}

void GestionExplosion::setMonJoueur(TankNormal* monJoueur)
{
    m_monJoueur = monJoueur;
}
