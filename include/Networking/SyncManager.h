#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#include <SFML/Network.hpp>
#include "Crystal.h"

#include <string>
#include <map>

#define MAX_PLAYERS 16

#include "Object.h"

class strless {
   public:
      bool operator() (const std::string & first, const std::string & second ) const  {
         return first < second;
      }
};

class SyncManager
{
    public:
        static void init();
        static void connectToServer( const sf::IpAddress& address );

        static sf::SocketSelector socketSelector;
        static sf::TcpSocket tcpSocket;
        static sf::UdpSocket udpSocket;
        static sf::IpAddress address;
        static short unsigned int serverPort;
        static short unsigned int localUDPPort;
        static bool connected;
        static sf::Packet packet;
        static sf::SocketSelector selector;

        static void sendTCPMessage( const char * buffer, size_t size );
        static void sendUDPMessage( const char * buffer, size_t size );

        static void sendTCPMessage( sf::Packet packetToSend );
        static void sendUDPMessage( sf::Packet packetToSend );

        static void receivePackets();
        static void parseBuffer();

        static int myPlayerID;
        static bool players[MAX_PLAYERS];

        static void triggerServerEvent( const char * eventName );
        static void registerServerEvent( const char * eventName, sf::Uint16 id );
        static void requestServerEvents();


        static void requestClientEvent( const char * eventName );
        static void registerClientEvent( const char * eventName, sf::Uint16 id );

        static void registerObjectToServer( Object * obj );

        static sf::Uint16 getServerEventCode( const char * eventName );
        static std::string getClientEventName( sf::Uint16 eventCode );
    protected:

    private:
        SyncManager();
        static std::map<std::string,sf::Uint16,strless> serverEvents;
        static std::map<std::string,sf::Uint16,strless> clientEvents;
};

#endif // SYNCMANAGER_H
