/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#include <irrlicht/irrlicht.h>
#include "BoutonManette.h"

BoutonManette::BoutonManette()
{
    m_X_estClique = false;
    m_O_estClique = false;
    m_R1_estClique = false;
    m_L1_estClique = false;

    m_X_dernierEtat = LEVE;
    m_O_dernierEtat = LEVE;
    m_R1_dernierEtat = LEVE;
    m_L1_dernierEtat = LEVE;
}

void BoutonManette::miseAJour(const irr::SEvent::SJoystickEvent& joystickState)
{
    //X
    if(joystickState.IsButtonPressed(X))
    {
        if(m_X_dernierEtat == LEVE)
        {
            m_X_estClique = true;
            m_X_dernierEtat = PRESSE;
        }
    }

    else
    {
        m_X_estClique = false;
        m_X_dernierEtat = LEVE;
    }

    //O
    if(joystickState.IsButtonPressed(O))
    {
        if(m_O_dernierEtat == LEVE)
        {
            m_O_estClique = true;
            m_O_dernierEtat = PRESSE;
        }
    }
    else
    {
        m_O_estClique = false;
        m_O_dernierEtat = LEVE;
    }

    //L1
    if(joystickState.IsButtonPressed(L1))
    {
        if(m_L1_dernierEtat == LEVE)
        {
            m_L1_estClique = true;
            m_L1_dernierEtat = PRESSE;
        }
    }
    else
    {
        m_L1_estClique = false;
        m_L1_dernierEtat = LEVE;
    }

    //R1
    if(joystickState.IsButtonPressed(R1))
    {
        if(m_R1_dernierEtat == LEVE)
        {
            m_R1_estClique = true;
            m_R1_dernierEtat = PRESSE;
        }
    }
    else
    {
        m_R1_estClique = false;
        m_R1_dernierEtat = LEVE;
    }
}

bool BoutonManette::X_estClique()
{
    bool etatBouton = false;
    if(m_X_estClique)
    {
        etatBouton = true;
        m_X_estClique = false;
    }
    return etatBouton;
}

bool BoutonManette::O_estClique()
{
    bool etatBouton = false;
    if(m_O_estClique)
    {
        etatBouton = true;
        m_O_estClique = false;
    }
    return etatBouton;
}

bool BoutonManette::L1_estClique()
{
    bool etatBouton = false;
    if(m_L1_estClique)
    {
        etatBouton = true;
        m_L1_estClique = false;
    }
    return etatBouton;
}

bool BoutonManette::R1_estClique()
{
    bool etatBouton = false;
    if(m_R1_estClique)
    {
        etatBouton = true;
        m_R1_estClique = false;
    }
    return etatBouton;
}
