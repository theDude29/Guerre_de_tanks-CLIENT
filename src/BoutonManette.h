/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifndef BOUTONMANETTE
#define BOUTONMANETTE

#include <irrlicht/irrlicht.h>

enum ETAT_BOUTON {LEVE, PRESSE};
enum BOUTON {X = 1, O = 2, L1 = 4, R1 = 5};

class BoutonManette
{
public:
    BoutonManette();
    void miseAJour(const irr::SEvent::SJoystickEvent& joystickState);
    bool X_estClique();
    bool O_estClique();
    bool R1_estClique();
    bool L1_estClique();

private:
    bool m_X_estClique, m_O_estClique, m_R1_estClique, m_L1_estClique;
    bool m_X_dernierEtat, m_O_dernierEtat, m_R1_dernierEtat, m_L1_dernierEtat;
};
#endif // BOUTONMANETTE
