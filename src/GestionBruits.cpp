/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#include "GestionBruits.h"
#include "Joueur.h"
#include "MyGui.h"

GestionBruits::GestionBruits(std::vector<Joueur*> listeJoueur, sf::Int32 monID, MyGui* myGui)
{
    m_myGui = myGui;

    //son
    m_soundBufferTire.loadFromFile("son/tire.wav");
    m_sonTire.setBuffer(m_soundBufferTire);
    m_sonTire.setVolume(50);
    m_volumeSonTire = 50;
    //
    m_listeJoueur = listeJoueur;

    m_soundBufferChenille.loadFromFile("son/chenille.wav");
    m_sonChenille.setBuffer(m_soundBufferChenille);
    for(unsigned int i = 0; i<listeJoueur.size(); ++i)
    {
        m_sonsChenille.push_back(m_sonChenille);
    }
    m_volumeSonChenille = 3;
    //
    m_soundBufferImpossible.loadFromFile("son/impossible.wav");
    m_sonImpossible.setBuffer(m_soundBufferImpossible);
    m_sonImpossible.setVolume(40);
    //
    m_soundBufferRecharger.loadFromFile("son/recharger.wav");
    m_sonRecharger.setBuffer(m_soundBufferRecharger);
    m_sonRecharger.setVolume(80);
    m_volumeSonRechargement = 80;
    //
    m_soundBufferExplosion.loadFromFile("son/explosion.wav");
    m_sonExplosion.setBuffer(m_soundBufferExplosion);
    m_sonExplosion.setVolume(100);
    m_volumeSonExplosion = 100;
    //
    m_soundBufferChenille_extrait.loadFromFile("son/chenille_extrait.wav");
    m_sonChenille_extrait.setBuffer(m_soundBufferChenille_extrait);

    m_monID = monID;
    m_monJoueur = listeJoueur[m_monID];
}

void GestionBruits::jouerSon(sf::Int32 id, TypeMessage action)
{
    if(action == CHANGEMENT_POSITION)
    {
        if(m_monID == id)
        {
            m_sonsChenille[m_monID].setVolume(m_volumeSonChenille);
        }

        else
        {
            Joueur* source = m_listeJoueur[id];
            irr::core::vector3df longueur = m_monJoueur->getPosition() - source->getPosition();
            irr::f32 distanceDeLaSource = sqrt(longueur.X*longueur.X + longueur.Y*longueur.Y + longueur.Z*longueur.Z);
            if(distanceDeLaSource > 2000) distanceDeLaSource = 2000;
            irr::s32 volumeDuBruit = m_volumeSonChenille - (m_volumeSonChenille * distanceDeLaSource / 2000);
            m_sonsChenille[id].setVolume(volumeDuBruit);
        }

        if(m_sonsChenille[id].getStatus() == sf::Sound::Paused || m_sonsChenille[id].getStatus() == sf::Sound::Stopped)
        {
            m_sonsChenille[id].play();
        }
    }

    if(action == ARRETER_BOUGER)
    {
        m_sonsChenille[id].pause();
    }

    if(action == PACKET_MUNITION_CONSOMME)
    {
        if(m_monID == id)
        {
            m_sonRecharger.setVolume(m_volumeSonRechargement);
            m_sonRecharger.play();
        }

        else
        {
            Joueur* source = m_listeJoueur[id];
            irr::core::vector3df longueur = m_monJoueur->getPosition() - source->getPosition();
            irr::f32 distanceDeLaSource = sqrt(longueur.X*longueur.X + longueur.Y*longueur.Y + longueur.Z*longueur.Z);
            if(distanceDeLaSource > 2000) distanceDeLaSource = 2000;
            irr::s32 volumeDuBruit = m_volumeSonRechargement - (m_volumeSonRechargement * distanceDeLaSource / 2000);
            m_sonRecharger.setVolume(volumeDuBruit);
        }

        m_sonRecharger.play();
    }

    if(action == TIRE)
    {
        if(id == m_monID)
        {
            m_sonTire.setVolume(m_volumeSonTire);
            m_sonTire.play();
        }

        else
        {
            Joueur* source = m_listeJoueur[id];
            irr::core::vector3df longueur = m_monJoueur->getPosition() - source->getPosition();
            irr::f32 distanceDeLaSource = sqrt(longueur.X*longueur.X + longueur.Y*longueur.Y + longueur.Z*longueur.Z);
            if(distanceDeLaSource > 5000) distanceDeLaSource = 5000;
            irr::s32 volumeDuBruit = m_volumeSonTire - (m_volumeSonTire * distanceDeLaSource / 5000);
            m_sonTire.setVolume(volumeDuBruit);
        }

        m_sonTire.play();
    }
}

void GestionBruits::jouerSonExplosion(irr::core::vector3df posExplo)
{
    irr::core::vector3df longueur = m_monJoueur->getPosition() - posExplo;
    irr::f32 distanceDeLaExplo = sqrt(longueur.X*longueur.X + longueur.Y*longueur.Y + longueur.Z*longueur.Z);
    if(distanceDeLaExplo > 8000) distanceDeLaExplo = 8000;
    irr::s32 volumeDuBruit = m_volumeSonExplosion - (m_volumeSonExplosion * distanceDeLaExplo / 8000);
    m_sonExplosion.setVolume(volumeDuBruit);
    m_sonExplosion.play();
}

void GestionBruits::setVolumeChenille(irr::s32 volume)
{
    m_volumeSonChenille = volume;
    m_sonChenille_extrait.setVolume(volume);
    m_sonChenille_extrait.play();
}

void GestionBruits::setVolumeRechargement(irr::s32 volume)
{
    m_volumeSonRechargement = volume;
    m_sonRecharger.setVolume(volume);
    m_sonRecharger.play();
}

void GestionBruits::setVolumeTire(irr::s32 volume)
{
    m_volumeSonTire = volume;
    m_sonTire.setVolume(volume);
    m_sonTire.play();
}

void GestionBruits::setVolumeExplosion(irr::s32 volume)
{
    m_volumeSonExplosion = volume;
    m_sonExplosion.setVolume(volume);
    m_sonExplosion.play();
}
