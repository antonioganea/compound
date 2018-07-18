#include "SyncManager.h"

#include "StageManager.h"

#include <string.h>
#include <iostream>

#include "Events.h"
#include "LuaConsole.h"

#include "DebugLog.h"


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

std::string SyncManager::nickname;

std::map<std::string,sf::Uint16,strless> SyncManager::serverEvents;
std::map<std::string,sf::Uint16,strless> SyncManager::clientEvents;

void SyncManager::init(){
    //SyncManager::packageBuffer = new char[128];
    //SyncManager::options = new char[128];
    for ( int i = 0; i < MAX_PLAYERS; i++ ){
        SyncManager::players[i] = false;
        //SyncManager::crystals[i]= NULL;
    }
    DebugLog::debug("SyncManager Initialized!");
    SyncManager::myPlayerID = -1;
    SyncManager::serverPort = 4474;
    SyncManager::localUDPPort = 30125;
    SyncManager::connected = false;
    nickname = "";
}

void SyncManager::connectToServer( const sf::IpAddress& _address ){
    DebugLog::info( "Attempting to connect to server on port : %d", SyncManager::serverPort );
    sf::Socket::Status status = SyncManager::tcpSocket.connect( _address, SyncManager::serverPort, sf::Time::Zero );
    if ( status == sf::Socket::Status::Done ){
        DebugLog::info( "Successfully connected to server! ( TCP )" );
        SyncManager::connected = true;
        //SyncManager::tcpSocket.setBlocking(false);
    }
    else
        DebugLog::error( "Failed to connect to server!" );

    // UDP socket:
    SyncManager::address = _address;
    //SyncManager::serverPort = 4474;
    // bind the socket to a port
    if (SyncManager::udpSocket.bind(SyncManager::localUDPPort) != sf::Socket::Done){
        DebugLog::error( "Problem binding UDP socket to port %hu", SyncManager::localUDPPort );
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
    //SyncManager::requestClientEvent("beemo:pineapple");
}


void SyncManager::sendTCPMessage( const char* buffer, size_t size ){
    SyncManager::tcpSocket.send( buffer, size );
}

void SyncManager::sendUDPMessage( const char* buffer, size_t size ){
    SyncManager::udpSocket.send( buffer, size, SyncManager::address, SyncManager::serverPort );
}

void SyncManager::sendTCPMessage(sf::Packet packetToSend){
    SyncManager::tcpSocket.send( packetToSend );
}

void SyncManager::sendUDPMessage(sf::Packet packetToSend){
    SyncManager::udpSocket.send( packetToSend, SyncManager::address, SyncManager::serverPort );
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
        DebugLog::trace( "Triggered server event %s", eventName );
    }
    else{
        DebugLog::error( "Packet not sent, Event not found : %s", eventName );
    }
}

sf::Uint16 SyncManager::getServerEventCode( const char * eventName ){
    sf::Uint16 eventCode = 0;

    std::map<std::string,sf::Uint16,strless>::iterator it;
    it = serverEvents.find(eventName);
    if (it != serverEvents.end()){
        eventCode = it->second;
        return eventCode;
    }
    return eventCode;
}

std::string SyncManager::getClientEventName(sf::Uint16 eventCode){
    std::string mystr = "";
    for ( std::map<std::string,sf::Uint16,strless>::const_iterator i = clientEvents.begin(); i != clientEvents.end(); ++i ) {
        if ( i->second == eventCode ){
            mystr = i->first;
            return mystr;
        }
    }
    return mystr;
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
    DebugLog::debug( "Registered server event %s %d", eventName, id );
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
                DebugLog::error("ERORR ON PACKAGE RECEIVEING (TCP) !!!!!!!!!!!!!!!!");
            else if ( status == sf::Socket::Status::Partial )
                DebugLog::debug("Partial receive might cause bugs!!!!!!!!!!!!!!!!!!!!!!!!");
            else if ( status != sf::Socket::NotReady )
                DebugLog::trace("SOME PACKET ( NOT INTERPRETED / FINISHED ) %d", status);
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

    DebugLog::debug("Requested client event %s",eventName);
}

void SyncManager::registerObjectToServer(Object* obj){
    sf::Packet newPacket;
    newPacket = obj->generateObjectPacket();
    SyncManager::tcpSocket.send(newPacket);

    DebugLog::debug( "Sent client object registration packet to server on id %d", obj->getClientID() );
}


void SyncManager::registerClientEvent(const char* eventName, sf::Uint16 id){
    clientEvents[eventName] = id;
    DebugLog::debug( "Registered client event %s %d", eventName, id );
}


void SyncManager::parseBuffer(){

    //printf( "Received : %s\n", packageBuffer );

    sf::Uint16 packetCode;

    receivePacket >> packetCode;

    DebugLog::trace("Received something ( packetID %d )",packetCode);

    switch ( packetCode ){
        case C_SEND_SERVER_EVENTS:{ // Server Events
            std::string eventName;
            sf::Uint16 eventCode;
            while ( !receivePacket.endOfPacket() ){
                receivePacket >> eventName >> eventCode;
                registerServerEvent(eventName.c_str(),eventCode);
            }
            break;
        }
        case C_CLIENT_EVENT_ACKNOWLEDGEMENT:{ // Client Events
            std::string eventName;
            sf::Uint16 eventCode;
            receivePacket >> eventName >> eventCode;
            registerClientEvent(eventName.c_str(),eventCode);
            break;
        }
        case C_RUN_CLIENT_SCRIPTS:{
            LuaConsole::execute("resources/crystal/client.lua");
            LuaConsole::triggerUpdate = true;
            break;
        }

        case C_REGISTRATION_CODE:{
            //C_REGISTRATION_CODE << clientID << serverID;
            sf::Uint16 clientID, serverID;
            receivePacket >> clientID >> serverID;

            Object * obj = StageManager::gameState->getObject(clientID);
            obj->setServerID(serverID);
            obj->setSynced(true);
            StageManager::gameState->bindServerIDtoClientObject(serverID,clientID);

            DebugLog::debug("Client object %d is now synced with serverID %d", clientID, serverID );

            break;
        }
        case C_SINGLE_OBJECT:{
            //newPacket << C_SINGLE_OBJECT << serverID << x << y << vx << vy << friction << rotation << textureID;

            sf::Uint16 serverID, textureID;
            float x,y,vx,vy,friction,rotation;

            receivePacket >> serverID >> x >> y >> vx >> vy >> friction >> rotation >> textureID;

            Object * newObject = new Object();
            newObject->setPosition(x,y);
            newObject->setVelocity(vx,vy);
            newObject->setFriction(friction);
            newObject->setRotation(rotation);
            newObject->setTextureID(textureID);
            newObject->setServerID( serverID );
            newObject->setSynced(true);

            sf::Uint16 clientID = StageManager::gameState->registerClientObject( newObject );
            StageManager::gameState->bindServerIDtoClientObject( serverID, clientID );

            break;
        }

    case SHARED_POSITION:{
            sf::Uint16 serverID;
            float x,y;
            receivePacket >> serverID >> x >> y;
            Object * obj = StageManager::gameState->getObjectByServerID(serverID);
            //std::cout << "POSITION : " << serverID << ' ' << x << ' ' << y << std::endl;
            if ( obj != 0 ){
                obj -> setPosition(x,y);
            }
            break;
        }
        case SHARED_VELOCITY:{
            sf::Uint16 serverID;
            float vx,vy,x,y;
            receivePacket >> serverID >> vx >> vy >> x >> y;
            Object * obj = StageManager::gameState->getObjectByServerID(serverID);
            if ( obj != 0 ){
                obj -> setVelocity(vx,vy);
                obj -> setPosition(x,y);
            }
            break;
        }
        case SHARED_FRICTION:{
            sf::Uint16 serverID;
            float friction;
            receivePacket >> serverID >> friction;
            Object * obj = StageManager::gameState->getObjectByServerID(serverID);
            if ( obj != 0 ){
                obj -> setFriction(friction);
            }
            break;
        }
        case SHARED_ROTATION:{
            sf::Uint16 serverID;
            float rotation;
            receivePacket >> serverID >> rotation;
            Object * obj = StageManager::gameState->getObjectByServerID(serverID);
            if ( obj != 0 ){
                obj -> setRotation(rotation);
            }
            break;
        }
        case SHARED_TEXTUREID:{
            sf::Uint16 serverID;
            sf::Uint16 textureID;
            receivePacket >> serverID >> textureID;
            Object * obj = StageManager::gameState->getObjectByServerID(serverID);
            if ( obj != 0 ){
                obj -> setTextureID(textureID);
            }
            break;
        }
        case SHARED_KILL:{
            sf::Uint16 serverID;
            receivePacket >> serverID;
            Object * obj = StageManager::gameState->getObjectByServerID(serverID);
            if ( obj != 0 ){
                obj -> kill();
            }
            break;
        }

        default:{
            if ( packetCode >= 500 ){
                LuaConsole::triggerClientEvent(packetCode,receivePacket);
            }
        }


    }
}
