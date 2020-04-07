/* Guerre de Tank, bataille de tank en réseau. Copyright (C) 2018 Rémi Pérenne. 
Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier au titre des clauses de la Licence Publique Générale GNU, telle que publiée par la Free Software Foundation ; soit la version 2 de la Licence, ou (à votre discrétion) une version ultérieure quelconque. Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; sans même une garantie implicite de COMMERCIABILITE ou DE CONFORMITE A UNE UTILISATION PARTICULIERE. Voir la Licence Publique Générale GNU pour plus de détails. Vous devriez avoir reçu un exemplaire de la Licence Publique Générale GNU avec ce programme ; si ce n'est pas le cas, écrivez à la Free Software Foundation Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. */

#include "MyEventReceiver.h"
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include "infoPartie.h"

using namespace irr;

MyEventReceiver::MyEventReceiver(gui::IGUIEnvironment* gui, irr::ITimer* timer) : irr::IEventReceiver()
{
    //partie importante iirlicht
    m_timer = timer;

    //reseau
    m_socket.setBlocking(false);

    //gui
    m_textVersion = gui->addStaticText(L"-Version Client 1.0", core::rect<s32>(650, 110, 750, 130));
    m_textPresentation = gui->addStaticText(L"REJOINDRE UN SERVEUR:\n\nADRESSE:                                                      PORT:                                                  PSEUDO :",
                        core::rect<s32>(80, 200, 600, 600));
    m_ebAdresseIP = gui->addEditBox(L"127.0.0.1", core::rect<s32>(128, 225, 240, 245), true, 0, 1);
    m_ebPort = gui->addEditBox(L"50000", core::rect<s32>(320, 225, 420, 245), true, 0, 2);
    m_ebPseudo = gui->addEditBox(L"", core::rect<s32>(510, 225, 680, 245), true, 0, 3);
    m_bRejoindre = gui->addButton(core::rect<s32>(80, 270, 680, 320), 0, 5, L"REJOINDRE");
    m_textInfoConnectionServeur = gui->addStaticText(0, core::rect<s32>(0,0,500, 100));

    //anim
    m_animAttendreConnection[0] = L"";
    m_animAttendreConnection[1] = L".";
    m_animAttendreConnection[2] = L"..";
    m_animAttendreConnection[3] = L"...";

    //autre
    m_textInfoConnexionServeur_contenu = L"";
    m_heureDernierAppuiTouchePrisEnCompte = 0;
    m_connecte = false;
    m_enAttenteDesAutreJoueurs = false;
    m_enAttenteDeLaCreationDesEquipes = false;

    m_ebAdresseIPFocused = false;
    m_ebPortFocused = false;
}

bool MyEventReceiver::OnEvent(const irr::SEvent& event)
{
    if(event.EventType == EET_GUI_EVENT)
    {
        s32 id = event.GUIEvent.Caller->getID();
        switch(event.GUIEvent.EventType)
        {
        case gui::EGET_ELEMENT_FOCUSED:
            m_ebPortFocused = false;
            m_ebAdresseIPFocused = false;

            if(id == 1)
                m_ebAdresseIPFocused = true;
            if(id == 2)
                m_ebPortFocused = true;
            return false;

        case gui::EGET_BUTTON_CLICKED:
            tentativeDeConnexion();
            return true;

        default : return false;
        }

    }


    if(event.EventType == EET_KEY_INPUT_EVENT)
    {
        //JE FAIS CE SYSTEME PARCE QUE SINON JE
        //N'ARRIVE PAS A ECRIRE DE "7" DANS
        //EDIT BOX (GROS WTF)

        irr::EKEY_CODE key = event.KeyInput.Key;
        if(key == 16 && event.KeyInput.Shift)
        {
            if(m_timer->getTime() > m_heureDernierAppuiTouchePrisEnCompte  + 300)
            {
                m_heureDernierAppuiTouchePrisEnCompte = m_timer->getTime();
                if(m_ebAdresseIPFocused)
                {
                    std::wstring textDejaLa = m_ebAdresseIP->getText();
                    textDejaLa += L"7";
                    m_ebAdresseIP->setText(textDejaLa.c_str());
                }

                if(m_ebPortFocused)
                {
                    std::wstring textDejaLa = m_ebPort->getText();
                    textDejaLa += L"7";
                    m_ebPort->setText(textDejaLa.c_str());
                }
            }
        }
    }

    return false;
}

bool MyEventReceiver::miseAJour(irr::core::dimension2d<u32> screenSize)
{
    if(m_connecte)
    {
        if(m_enAttenteDesAutreJoueurs)
            miseAJourEtatDesConnection();

        if(m_enAttenteDeLaCreationDesEquipes && laPartieCommence())
            return true;

        sf::Packet packet;
        if(m_socket.receive(packet) == sf::Socket::Disconnected)
        {
            m_connecte = false;
            m_textInfoConnectionServeur->setText(L"Le serveur a été férmé vous etes donc deconnecté");
            m_bRejoindre->setEnabled(true);
            m_socket.disconnect();
        }

        //animation (., .., ...)
        if(m_timer->getTime() > m_heureDerniereRafraichissementAnimAttendreConnection + 300)
        {
            m_heureDerniereRafraichissementAnimAttendreConnection = m_timer->getTime();

            core::stringw text = m_textInfoConnexionServeur_contenu;
            text += m_animAttendreConnection[m_indiceAnimAttendreConnection];
            m_textInfoConnectionServeur->setText(text.c_str());

            m_indiceAnimAttendreConnection++;
            if(m_indiceAnimAttendreConnection > 3) m_indiceAnimAttendreConnection = 0;
        }
    }

    //GUI
    s32 screenSize_middleWidth = screenSize.Width/2;
    s32 screenSize_middleHeight = screenSize.Height/2;

    core::position2di positionElement;

    positionElement.X = screenSize_middleWidth + 250;
    positionElement.Y =  screenSize_middleHeight - 190;
    m_textVersion->setRelativePosition(positionElement);

    positionElement.X = screenSize_middleWidth - 320;
    positionElement.Y =  screenSize_middleHeight - 100;
    m_textPresentation->setRelativePosition(positionElement);

    positionElement.Y =  screenSize_middleHeight - 75;

    positionElement.X = screenSize_middleWidth - 270;
    m_ebAdresseIP->setRelativePosition(positionElement);

    positionElement.X = screenSize_middleWidth - 80;
    m_ebPort->setRelativePosition(positionElement);

    positionElement.X = screenSize_middleWidth + 110;
    m_ebPseudo->setRelativePosition(positionElement);

    positionElement.Y =  screenSize_middleHeight - 20;
    positionElement.X = screenSize_middleWidth - 320;
    m_bRejoindre->setRelativePosition(positionElement);

    positionElement.Y =  screenSize_middleHeight + 60;
    positionElement.X = screenSize_middleWidth - 200;
    m_textInfoConnectionServeur->setRelativePosition(positionElement);

    return false;
}

void MyEventReceiver::miseAJourEtatDesConnection()
{
    sf::Packet packet;
    if(m_socket.receive(packet) == sf::Socket::Done)
    {
        sf::Int32 nbJoueurConnecte, nbJoueurAttendue;
        packet>>nbJoueurConnecte;
        packet>>nbJoueurAttendue;

        m_textInfoConnexionServeur_contenu = L"En attente des autres clients : (";
        m_textInfoConnexionServeur_contenu += nbJoueurConnecte;
        m_textInfoConnexionServeur_contenu += L"/";
        m_textInfoConnexionServeur_contenu += nbJoueurAttendue;
        m_textInfoConnexionServeur_contenu += L" client ont rejoint la partie)";

        m_textInfoConnectionServeur->setText(m_textInfoConnexionServeur_contenu.c_str());

        if(nbJoueurAttendue == nbJoueurConnecte)
        {
            m_textInfoConnexionServeur_contenu = L"Création des équipes";
            m_enAttenteDesAutreJoueurs = false;
            m_enAttenteDeLaCreationDesEquipes = true;
        }

        m_nbJoueursConnecte = nbJoueurConnecte;
    }
}

void MyEventReceiver::tentativeDeConnexion()
{
    std::wstring text_wstring;
    std::string text_string;

    text_wstring = m_ebAdresseIP->getText();
    text_string.append(text_wstring.begin(), text_wstring.end());
    sf::IpAddress adresse = text_string;

    text_wstring = m_ebPort->getText();
    unsigned short port = wcstol(text_wstring.c_str(), 0, 10);

    text_wstring = m_ebPseudo->getText();
    sf::String pseudo = "";
    pseudo = text_wstring;

    if(pseudo != "")
    {
        //JE MET LA SOCKET EN BLOQUANTE POUR LA CONNECTION
        //SINON IL FAUT DE APPUYER 2 FOIS SUR LE BOUTON DE CONNECTION POUR
        //SE CONNECTERER (WTF)
        m_socket.setBlocking(true);
        if(m_socket.connect(adresse, port) == sf::Socket::Done)
        {
            sf::Packet packet;
            packet<<pseudo;
            m_socket.send(packet);

            packet.clear();
            m_socket.receive(packet);
            packet>>m_id;
            m_socket.setBlocking(false);

            m_bRejoindre->setEnabled(false);

            m_textInfoConnectionServeur->setText(L"Connexion au serveur réussi");
            m_connecte = true;
            m_enAttenteDesAutreJoueurs = true;
        }
        else
            m_textInfoConnectionServeur->setText(L"Impossible de se connecter au serveur.\nVérifier si le serveur est bien fonctionnel\nSi oui, vérifier si l'adresse IP et le port sont bien les bons");
    }

    else
    {
        m_textInfoConnectionServeur->setText(L"Vous devez donner un pseudo pour vous connecter à un serveur");
    }
}

bool MyEventReceiver::laPartieCommence()
{
    infoPartie info;
    sf::Packet packet;
    if(m_socket.receive(packet) == sf::Socket::Done)
    {
        packet>>info;
        m_listeClients = info.listeCLients;
        m_posPaquesMunition = info.listePositionPaqueMunition;
        m_intervalleRafraichissement = info.limiteRafraichissement;
        m_nbEliminationGagnePartie = info.nbEliminationGagnePartie;

        m_bRejoindre->remove();
        m_ebAdresseIP->remove();
        m_ebPort->remove();
        m_ebPseudo->remove();
        m_textInfoConnectionServeur->remove();
        m_textPresentation->remove();
        m_textVersion->remove();

        return true;
    }
    return false;
}

unsigned short MyEventReceiver::getPort()
{
    return m_socket.getLocalPort();
}

sf::Uint32 MyEventReceiver::getID()
{
    return m_id;
}

sf::Uint32 MyEventReceiver::getNbJoueursConnecte()
{
    return m_nbJoueursConnecte;
}

unsigned short MyEventReceiver::getPortServeur()
{
    return m_socket.getRemotePort();
}

sf::IpAddress MyEventReceiver::getAdresseServeur()
{
    return m_socket.getRemoteAddress();
}

std::vector<Client> MyEventReceiver::getListeClients()
{
    return m_listeClients;
}

std::vector<core::vector3df> MyEventReceiver::getPosPaquesMunition()
{
    return m_posPaquesMunition;
}

sf::Int32 MyEventReceiver::getIntervalleRafraichissement()
{
    return m_intervalleRafraichissement;
}

sf::Int32 MyEventReceiver::getNbEliminationGagnePartie()
{
    return m_nbEliminationGagnePartie;
}
