/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#include "infoPositionRotationJoueur.h"

bool operator==(struct infoPositionRotationJoueur const& a, struct infoPositionRotationJoueur const& b)
{
    if(a.pos == b.pos && a.rotCorps == b.rotCorps && a.rotCanon == b.rotCanon)
    {
        return true;
    }

    return false;
}

bool operator!=(struct infoPositionRotationJoueur const& a, struct infoPositionRotationJoueur const& b)
{
    if((a == b) == false)
    {
        return true;
    }

    return false;
}

sf::Packet& operator<<(sf::Packet& packet, const infoPositionRotationJoueur& a)
{
    irr::core::vector3df pos = a.pos, rotCorps = a.rotCorps, rotCanon = a.rotCanon;
    return packet<<pos.X<<pos.Y<<pos.Z<<rotCorps.X<<rotCorps.Y<<rotCorps.Z<<rotCanon.X<<rotCanon.Z<<rotCanon.Y;
}

sf::Packet& operator>>(sf::Packet& packet, infoPositionRotationJoueur& a)
{
    return packet>>a.pos.X>>a.pos.Y>>a.pos.Z>>a.rotCorps.X>>a.rotCorps.Y>>a.rotCorps.Z>>a.rotCanon.X>>a.rotCanon.Z>>a.rotCanon.Y;
}
