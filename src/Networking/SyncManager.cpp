#include "SyncManager.h"

#include "StageManager.h"

#include <string.h>
#include <iostream>


sf::TcpSocket SyncManager::tcpSocket;
sf::UdpSocket SyncManager::udpSocket;
sf::IpAddress SyncManager::address;
short unsigned int SyncManager::serverPort;
short unsigned int SyncManager::localUDPPort;
bool SyncManager::connected;
//char * SyncManager::packageBuffer;
sf::Packet SyncManager::packet;
sf::SocketSelector SyncManager::socketSelector;

//Crystal * SyncManager::crystals[MAX_PLAYERS];
//char * SyncManager::options;
bool SyncManager::players[MAX_PLAYERS];
int SyncManager::myPlayerID;

std::map<std::string,sf::Uint16,strless> SyncManager::serverEvents;
std::map<std::string,sf::Uint16,strless> SyncManager::clientEvents;

void SyncManager::init(){
    //SyncManager::packageBuffer = new char[128];
    //SyncManager::options = new char[128];
    for ( int i = 0; i < MAX_PLAYERS; i++ ){
        SyncManager::players[i] = false;
        //SyncManager::crystals[i]= NULL;
    }
    std::cout << "SyncManager Initialized!" << std::endl;
    SyncManager::myPlayerID = -1;
    SyncManager::serverPort = 4474;
    SyncManager::localUDPPort = 30125;
    SyncManager::connected = false;
}

void SyncManager::connectToServer( const sf::IpAddress& _address ){
    std::cout << "Attempting to connect to server.. on port " << SyncManager::serverPort << std::endl;
    sf::Socket::Status status = SyncManager::tcpSocket.connect( _address, SyncManager::serverPort, sf::Time::Zero );
    if ( status == sf::Socket::Status::Done ){
        std::cout << "Successfully connected to server! ( TCP )" << std::endl;
        SyncManager::connected = true;
        //SyncManager::tcpSocket.setBlocking(false);
    }
    else
        std::cout << "Failed to connect to server!" << std::endl;

    // UDP socket:
    SyncManager::address = _address;
    //SyncManager::serverPort = 4474;
    // bind the socket to a port
    if (SyncManager::udpSocket.bind(SyncManager::localUDPPort) != sf::Socket::Done){
        std::cout << "Problem binding UDP socket to port " << SyncManager::localUDPPort << std::endl;
    }
    else{
        std::cout << "Successfuly bound UDP socket" << std::endl;
    }
    //SyncManager::udpSocket.setBlocking(false);

    SyncManager::socketSelector.add(SyncManager::tcpSocket);
    SyncManager::socketSelector.add(SyncManager::udpSocket);

    //Now request server events
    SyncManager::requestServerEvents();

    //Let's allocate a client event
    SyncManager::requestClientEvent("beemo:pineapple");
}


void SyncManager::sendTCPMessage( const char* buffer, size_t size ){
    SyncManager::tcpSocket.send( buffer, size );
}

void SyncManager::sendUDPMessage( const char* buffer, size_t size ){
    SyncManager::udpSocket.send( buffer, size, SyncManager::address, SyncManager::serverPort );
}

void SyncManager::triggerServerEvent( const char * eventName ){
    sf::Packet packet;
    //packet << events[ eventName ];
    //SyncManager::tcpSocket.send(packet);

    sf::Uint16 eventCode = -1;

    std::map<std::string,sf::Uint16,strless>::iterator it;
    it = serverEvents.find(eventName);
    if (it != serverEvents.end()){
        eventCode = it->second;
        packet << eventCode;
        SyncManager::tcpSocket.send(packet);
        std::cout << "Triggered server event " << eventName << std::endl;
    }
    else{
        std::cout << "ERROR : Packet not sent, Event not found : "  << eventName << std::endl;
    }
}
void SyncManager::registerServerEvent( const char* eventName, sf::Uint16 id )
{
    //hashmap.add(eventname,id);

    /*
    for ( map<string,int,strless>::const_iterator i = myMap.begin(); i != myMap.end(); ++i ) {
        cout << (*i).first << " " << (*i).second << endl;
    }*/
    //cout << myMap.find("onPlayerDropped")->first;
    //myMap["onPlayerJoined"] = 1;

    serverEvents[eventName] = id;
    std::cout << "Registered server event " << eventName << " " << id << std::endl;
}

void SyncManager::requestServerEvents(){
    packet.clear();
    sf::Uint16 packetCode = 1;
    packet << packetCode;
    sf::Socket::Status status = SyncManager::tcpSocket.send(packet);
    std::cout << status << ' ' <<sf::Socket::Status::Done << std::endl;
}

//**********************************RECEIVING PART**************************************

sf::Packet receivePacket;

void SyncManager::receivePackets(){
    if ( !SyncManager::connected )
        return;

    sf::Socket::Status status;

    //TCP RECEIVING PART ---------------------------------------------------------------
    //TODO : find a way to write this without toggling setBlocking() twice every frame
    SyncManager::tcpSocket.setBlocking(false); // TODO : GET RID OF THIS
    //status = SyncManager::tcpSocket.receive(SyncManager::packageBuffer,128,received);
//puts("H1");
    //if ( SyncManager::socketSelector.isReady(SyncManager::tcpSocket) ){

        status = SyncManager::tcpSocket.receive(receivePacket);
        if ( status == sf::Socket::Status::Done ){
            SyncManager::parseBuffer();

    /*
            std::cout << "RECEIVED A PACKET (TCP)" << received << " bytes" << std::endl;
            for ( int i = 0; i < received; i++ ){
                std::cout << (int)(SyncManager::packageBuffer[i]) << " ";
            }std::cout << std::endl;
    */

        }
        else{
            if ( status == sf::Socket::Status::Error )
                std::cout << "ERORR ON PACKAGE RECEIVEING (TCP) !!!!!!!!!!!!!!!!" << std::endl;
            else if ( status == sf::Socket::Status::Partial )
                std::cout << "Partial receive might cause bugs!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            else if ( status != sf::Socket::NotReady )
                std::cout << "SOME PACKET ( NOT INTERPRETED / FINISHED ) " << status << std::endl;
            // TODO : in case of other crashes, insert disconnected status error code and inspect further
        }
    //}
    SyncManager::tcpSocket.setBlocking(true); // TODO : GET RID OF THIS
//puts("H2");
    //END OF TCP RECEIVING PART ----------------------------------------------------------

/*
    //UDP RECEIVING PART ---------------------------------------------------------------
    //TODO : find a way to write this without toggling setBlocking() twice every frame
    SyncManager::udpSocket.setBlocking(false); // TODO : GET RID OF THIS
    sf::IpAddress _address = SyncManager::address; // also try to get rid of this, passing as arguments overwrites the value
    unsigned short int _port = SyncManager::serverPort; // ... same
    status = SyncManager::udpSocket.receive(SyncManager::packageBuffer,128,received,_address,_port);

    if ( status == sf::Socket::Status::Done ){
        SyncManager::parseBuffer( received );

        std::cout << "RECEIVED A PACKET (UDP)" << received << " bytes" << std::endl;
        for ( int i = 0; i < received; i++ ){
            std::cout << (int)(SyncManager::packageBuffer[i]) << " ";
        }std::cout << std::endl;

    }
    else{
        if ( status == sf::Socket::Status::Error )
            std::cout << "ERORR ON PACKAGE RECEIVEING (UDP) !!!!!!!!!!!!!!!!" << std::endl;
        else if ( status == sf::Socket::Status::Partial )
            std::cout << "Partial receive might cause bugs!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        // TODO : in case of other crashes, insert disconnected status error code and inspect further
    }
    SyncManager::udpSocket.setBlocking(true); // TODO : GET RID OF THIS

    //END OF UDP RECEIVING PART ----------------------------------------------------------
*/
}

void SyncManager::requestClientEvent(const char* eventName){
    sf::Packet tempPacket;
    tempPacket << (sf::Uint16)(2);
    tempPacket << eventName;

    SyncManager::tcpSocket.send(tempPacket);

    std::cout << "Requested client event " << eventName << std::endl;
}

void SyncManager::registerClientEvent(const char* eventName, sf::Uint16 id){
    clientEvents[eventName] = id;
    std::cout << "Registered client event " << eventName << " " << id << std::endl;
}


void SyncManager::parseBuffer(){

    //printf( "Received : %s\n", packageBuffer );

    printf("Received something! ");

    sf::Uint16 packetCode;

    receivePacket >> packetCode;

    printf("%d ",packetCode);

    switch ( packetCode ){
        case 1:{ // Server Events
            std::string eventName;
            sf::Uint16 eventCode;
            while ( !receivePacket.endOfPacket() ){
                receivePacket >> eventName >> eventCode;
                registerServerEvent(eventName.c_str(),eventCode);
            }
            break;
        }
        case 2:{ // Client Events
            std::string eventName;
            sf::Uint16 eventCode;
            receivePacket >> eventName >> eventCode;
            registerClientEvent(eventName.c_str(),eventCode);
        }

    }


}
//**************************************************************************************
/*
void SyncManager::input( sf::Event event ){

    if ( SyncManager::myPlayerID != -1 ){
        if ( SyncManager::crystals[SyncManager::myPlayerID] )
            SyncManager::crystals[SyncManager::myPlayerID]->input(event);
    }
}*/
