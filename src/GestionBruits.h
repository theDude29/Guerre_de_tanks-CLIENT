/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifndef GESTIONBRUIT
#define GESTIONBRUIT

#include <SFML/Audio.hpp>
#include "Joueur.h"

class MyGui;

class GestionBruits
{
public:
    GestionBruits(std::vector<Joueur*> listeJoueur, sf::Int32 monID, MyGui* myGui);
    void jouerSon(sf::Int32 id, TypeMessage action);
    void jouerSonExplosion(irr::core::vector3df posExplo);

    void setVolumeTire(irr::s32 volume);
    void setVolumeRechargement(irr::s32 volume);
    void setVolumeChenille(irr::s32 volume);
    void setVolumeExplosion(irr::s32 volume);

private:
    MyGui* m_myGui;

    std::vector<Joueur*> m_listeJoueur;
    Joueur* m_monJoueur;
    sf::Int32 m_monID;

    //son
    std::vector<sf::Sound> m_sonsChenille;
    sf::Int32 m_volumeSonRechargement, m_volumeSonChenille, m_volumeSonTire, m_volumeSonExplosion;
    sf::SoundBuffer m_soundBufferTire, m_soundBufferChenille, m_soundBufferImpossible, m_soundBufferRecharger, m_soundBufferExplosion, m_soundBufferChenille_extrait;
    sf::Sound m_sonTire, m_sonChenille, m_sonImpossible, m_sonRecharger, m_sonExplosion, m_sonChenille_extrait;
};

#endif // GESTIONBRUIT
