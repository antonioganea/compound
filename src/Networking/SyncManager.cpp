#include "SyncManager.h"

#include "StageManager.h"
/*
#include "Emeraldo.h"
#include "Rubie.h"
#include "Sapheer.h"
*/
#include <string.h>
#include <iostream>


sf::TcpSocket SyncManager::tcpSocket;
sf::UdpSocket SyncManager::udpSocket;
sf::IpAddress SyncManager::address;
short unsigned int SyncManager::serverPort;
short unsigned int SyncManager::localUDPPort;
bool SyncManager::connected;
char * SyncManager::packageBuffer;

//Crystal * SyncManager::crystals[MAX_PLAYERS];
//char * SyncManager::options;
bool SyncManager::players[MAX_PLAYERS];
int SyncManager::myPlayerID;

std::map<std::string,int,strless> SyncManager::serverEvents;

void SyncManager::init(){
    SyncManager::packageBuffer = new char[128];
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
        SyncManager::tcpSocket.setBlocking(false);
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
    SyncManager::udpSocket.setBlocking(false);
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

    sf::Int32 eventCode = -1;

    std::map<std::string,int,strless>::iterator it;
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
void SyncManager::registerServerEvent( const char* eventName, int id )
{
    //hashmap.add(eventname,id);

    /*
    for ( map<string,int,strless>::const_iterator i = myMap.begin(); i != myMap.end(); ++i ) {
        cout << (*i).first << " " << (*i).second << endl;
    }*/
    //cout << myMap.find("onPlayerDropped")->first;
    //myMap["onPlayerJoined"] = 1;

    serverEvents[eventName] = id;
}




//*********************************SENDING PART***************************************
/*
void SyncManager::sendCrystalType ( int type ){
    SyncManager::packageBuffer[0] = 1;//package type
    SyncManager::packageBuffer[1] = type;
    SyncManager::tcpSocket.send(SyncManager::packageBuffer,2);
    std::cout << "Sent crystal type successfully!" << std::endl;
}

void SyncManager::sendPosition(const sf::Vector2f& position){

    sf::Int32 x = position.x;
    sf::Int32 y = position.y;
    *SyncManager::packageBuffer = 2; // package code ( position = 2 )
    memcpy(SyncManager::packageBuffer+1,&x,sizeof(sf::Int32));
    memcpy(SyncManager::packageBuffer+sizeof(sf::Int32)+1,&y,sizeof(sf::Int32));


    SyncManager::tcpSocket.send(SyncManager::packageBuffer,sizeof(sf::Int32)*2+1);

/*
    sf::IpAddress _address = SyncManager::address;
    sf::Socket::Status errStatus = udpSocket.send(SyncManager::packageBuffer, sizeof(sf::Int32)*2+1, _address, SyncManager::serverPort);

    switch(errStatus){
    case sf::Socket::Status::Disconnected:
        std::cout << "UDP PACKAGE ERRORCODE -- socket disconnected" << std::endl;
        break;
    case sf::Socket::Status::Error:
        std::cout << "UDP PACKAGE ERRORCODE -- socket error" << std::endl;
        break;
    case sf::Socket::Status::NotReady:
        std::cout << "UDP PACKAGE ERRORCODE -- socket notready" << std::endl;
        break;
    case sf::Socket::Status::Partial:
        std::cout << "UDP PACKAGE ERRORCODE -- socket partial" << std::endl;
        break;

    case sf::Socket::Status::Done:
        std::cout << "UDP PACKAGE ERRORCODE -- socket done" << std::endl;
        break;

    }
*/

    //std::cout << "Address " << SyncManager::address.toString()  << " port : " << SyncManager::serverPort << std::endl;

    /*
    if ( != sf::Socket::Done){
        std::cout << "Error sending UDP packet - position" << std::endl;
    }*/
//}
/*
void SyncManager::sendShoot( const sf::Vector2f& velocity ){
    float x = velocity.x;
    float y = velocity.y;
    *SyncManager::packageBuffer = 3; // package code ( attack = 3 )
    memcpy(SyncManager::packageBuffer+1,&x,sizeof(float));
    memcpy(SyncManager::packageBuffer+sizeof(float)+1,&y,sizeof(float));
    SyncManager::tcpSocket.send(SyncManager::packageBuffer,sizeof(float)*2+1);
}

void SyncManager::sendUltimate(){
    SyncManager::packageBuffer[0] = 4; // package code ( ultimate = 4 / no payload )
    SyncManager::tcpSocket.send(SyncManager::packageBuffer,1);
}

void SyncManager::sendUltimate( char options ){
    SyncManager::packageBuffer[0] = 5; // package code ( ultimate = 5 / 1 byte payload )
    SyncManager::packageBuffer[1] = options;
    SyncManager::tcpSocket.send(SyncManager::packageBuffer,2);
}

void SyncManager::sendUltimate( float options ){
    SyncManager::packageBuffer[0] = 6; // package code ( ultimate = 6 / 4 byte payload )
    ((float*)(SyncManager::packageBuffer+1))[0] = options;
    SyncManager::tcpSocket.send(SyncManager::packageBuffer,5);
}

void SyncManager::sendLaserCollision( int player ){
    SyncManager::packageBuffer[0] = 7; // package code ( laserCollision / 4 byte payload )
    memcpy(SyncManager::packageBuffer+1,&player,4);
    SyncManager::tcpSocket.send(SyncManager::packageBuffer,5);
}

void SyncManager::sendShieldCollision( int player ){
    SyncManager::packageBuffer[0] = 8; // package code ( shield collision / 4 byte payload )
    memcpy(SyncManager::packageBuffer+1,&player,4);
    SyncManager::tcpSocket.send(SyncManager::packageBuffer,5);
}

void SyncManager::sendParticleCollision( int player ){
    SyncManager::packageBuffer[0] = 9; // package code ( particle collision / 4 byte payload )
    memcpy(SyncManager::packageBuffer+1,&player,4);
    SyncManager::tcpSocket.send(SyncManager::packageBuffer,5);
}
*/
//**************************************************************************************

//**********************************RECEIVING PART**************************************

void SyncManager::receivePackets(){
    if ( !SyncManager::connected )
        return;

    sf::Socket::Status status;
    std::size_t received;

    //TCP RECEIVING PART ---------------------------------------------------------------
    //TODO : find a way to write this without toggling setBlocking() twice every frame
    SyncManager::tcpSocket.setBlocking(false); // TODO : GET RID OF THIS
    status = SyncManager::tcpSocket.receive(SyncManager::packageBuffer,128,received);

    if ( status == sf::Socket::Status::Done ){
        //SyncManager::parseBuffer( received );


        std::cout << "RECEIVED A PACKET (TCP)" << received << " bytes" << std::endl;
        for ( int i = 0; i < received; i++ ){
            std::cout << (int)(SyncManager::packageBuffer[i]) << " ";
        }std::cout << std::endl;


    }
    else{
        if ( status == sf::Socket::Status::Error )
            std::cout << "ERORR ON PACKAGE RECEIVEING (TCP) !!!!!!!!!!!!!!!!" << std::endl;
        else if ( status == sf::Socket::Status::Partial )
            std::cout << "Partial receive might cause bugs!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        // TODO : in case of other crashes, insert disconnected status error code and inspect further
    }
    SyncManager::tcpSocket.setBlocking(true); // TODO : GET RID OF THIS

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

void SyncManager::parseBuffer( std::size_t received ){

    printf( "Received : %s\n", packageBuffer );

    /*
    char * cursor = SyncManager::packageBuffer;
    int player;

    //std::cout << "Received " << received << " bytes." << std::endl;

    while ( cursor - SyncManager::packageBuffer < received )
    {
        player = *cursor;
        cursor++;
        switch( *cursor ){ // Packet Code

            case 1:{ // Crystal Type
                std::cout << "Type 1 - crystal type" << std::endl;
                cursor++;
                SyncManager::players[player] = true;
                switch( *cursor ){ // Crystal Type Number
                    case 1:{
                        Emeraldo * emeraldo = new Emeraldo();
                        emeraldo->setSyncer(player);
                        StageManager::gameState->addEntity( emeraldo );
                        SyncManager::crystals[player] = emeraldo;
                        std::cout << "CREATED EMERALDO" << std::endl;
                        break;
                    }
                    case 2:{
                        Rubie * rubie = new Rubie();
                        rubie->setSyncer(player);
                        StageManager::gameState->addEntity( rubie );
                        SyncManager::crystals[player] = rubie;
                        std::cout << "CREATED RUBIE" << std::endl;
                        break;
                    }
                    case 3:{
                        Sapheer * sapheer = new Sapheer();
                        sapheer->setSyncer(player);
                        StageManager::gameState->addEntity( sapheer );
                        SyncManager::crystals[player] = sapheer;
                        std::cout << "CREATED SAPHEER" << std::endl;
                        break;
                    }
                }
                // tryna randomize
                SyncManager::crystals[player]->setPosition(sf::Vector2f( -50 + player*20, 0 ) );

                if ( player == SyncManager::myPlayerID ){
                    SyncManager::crystals[player]->setSyncable(true);
                }
                cursor++;
                break;
            }
            case 2:{ // Position package
                //std::cout << "Type 2 - position type" << std::endl;
                cursor++;
                sf::Int32 x,y;
                memcpy(&x,cursor,4);
                memcpy(&y,cursor+4,4);
                if ( SyncManager::players[player] )
                    SyncManager::crystals[player]->setPosition(sf::Vector2f(x,y));
                cursor += 8;
                break;
            }
            case 3:{ // Basic attack package
                std::cout << "Type 3 - basic attack --RECEIVED " << received << std::endl;
                cursor++;
                memcpy(SyncManager::options, cursor, 8 );
                SyncManager::crystals[player]->attack();
                cursor += 8;
                break;
            }
            case 4:{ // Ultimate package ( no options )
                std::cout << "Type 4 - ultimate ( no options )" << std::endl;
                cursor++;
                //memcpy(SyncManager::options,cursor,1);
                SyncManager::crystals[player]->ultimate();
                break;
            }
            case 5:{ // Ultimate package ( 1 byte )
                std::cout << "Type 5 - ultimate ( 1 byte )" << std::endl;
                cursor++;
                memcpy(SyncManager::options,cursor,1);
                SyncManager::crystals[player]->ultimate();
                cursor++;
                break;
            }
            case 6:{ // Ultimate package ( 4 bytes )
                std::cout << "Type 6 - ultimate ( 4 bytes )" << std::endl;
                cursor++;
                memcpy(SyncManager::options,cursor,4);
                SyncManager::crystals[player]->ultimate();
                cursor+=4;
                break;
            }
            case 10:{
                //std::cout << "Type 10 - Health Points" << std::endl;
                cursor++;
                int hp;
                memcpy(&hp,cursor,4);
                if ( SyncManager::crystals[player] )
                    SyncManager::crystals[player]->setHP(hp);
                //TODO
                cursor+=4;
                break;
            }
            case 11:{
                std::cout << "Type 11 - Disconnected player" << std::endl;
                cursor++;
                SyncManager::crystals[player]->markDead();
                SyncManager::crystals[player] = nullptr;
                SyncManager::players[player] = false;
                std::cout << "CALLED DEAD MARKUP" << std::endl;
                break;
            }
            case 12:{
                std::cout << "Type 12 - Received player ID" << std::endl;
                cursor++;
                SyncManager::myPlayerID = player;
                SyncManager::players[player] = true;
                break;
            }
            case 13:{ // Push Crystal
                //std::cout << "Type 13 - Push crystal type" << std::endl;
                cursor++;
                float x,y;
                memcpy(&x,cursor,4);
                memcpy(&y,cursor+4,4);
                if ( SyncManager::players[SyncManager::myPlayerID] )
                    SyncManager::crystals[myPlayerID]->pushCrystal(sf::Vector2f(x,y));
                cursor += 8;
                break;
            }
            case 14:{ // Circle Radius
                //std::cout << "Type 14 - Circle Radius" << std::endl;
                cursor++;
                int radius;
                memcpy(&radius,cursor,4);
                StageManager::gameState->circle.setRadius(radius);
                StageManager::gameState->circle.setOrigin(radius,radius);
                //std::cout << radius << std::endl;
                cursor+=4;
                break;
            }
            case 15:{ // Dead Crystal
                std::cout << "Type 15 - Dead Crystal" << std::endl;
                cursor++;

                SyncManager::crystals[player]->markDead();
                SyncManager::crystals[player] = nullptr;
                SyncManager::players[player] = false;
                break;
            }
            default:{
                std::cout << "Uninterpreted packet" << std::endl;
                cursor++;
            }
            //...
        }
    }*/
}
//**************************************************************************************
/*
void SyncManager::input( sf::Event event ){

    if ( SyncManager::myPlayerID != -1 ){
        if ( SyncManager::crystals[SyncManager::myPlayerID] )
            SyncManager::crystals[SyncManager::myPlayerID]->input(event);
    }
}*/
