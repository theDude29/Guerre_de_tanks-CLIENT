/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#include <irrlicht/irrlicht.h>
#include "MyCamera.h"

using namespace irr;

MyCamera::MyCamera(irr::scene::ICameraSceneNode* camera, bool equipe)
{
    m_camera = camera;
    if(equipe)
    {
        m_angleX = 3.14;
    }

    else
    {
        m_angleX = 0;
    }
    m_angleY = 4;
    m_rotateSpeed = 30;
}

void MyCamera::tournerHorizontal(bool gauche)
{
    if(gauche)
    {
        m_angleX += m_rotateSpeed * 0.001;
    }

    else
    {
        m_angleX -= m_rotateSpeed * 0.001;
    }
}

void MyCamera::tournerVertical(bool haut)
{
    if(haut)
    {
        m_angleY += m_rotateSpeed * 0.002;
        //on evite de depasser un certain niveau ca fait des probleme(WTF)
        if(m_angleY > 5)
        {
            m_angleY = 5;
        }
    }

    else
    {
        m_angleY -= m_rotateSpeed * 0.002;
        //on evite de depasser un certain niveau ca fait des probleme(WTF)
        if(m_angleY < 3)
        {
            m_angleY = 3;
        }
    }
}

void MyCamera::miseAJour(irr::core::vector3df posTank)
{
    //position
    core::vector3df posCam(0,200,0);

    posCam.X = 300 * cos(m_angleX);
    posCam.Z = 300 * sin(m_angleX);

    //target
    core::vector3df tarCam(0,0,0);

    tarCam.X = -300 * cos(m_angleX);
    tarCam.Z = -300 * sin(m_angleX);
    tarCam.Y = -300 * sin(m_angleY);

    //on aplique les changement
    m_camera->setPosition(posTank + posCam);
    m_camera->setTarget(posTank + tarCam);
}

void MyCamera::setRotateSpeed(irr::s32 rotateSpeed)
{
    m_rotateSpeed = rotateSpeed;
}

irr::core::vector3df MyCamera::getPosition()
{
    return m_camera->getPosition();
}
