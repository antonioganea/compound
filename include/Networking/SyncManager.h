#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#include <SFML/Network.hpp>
#include "Crystal.h"

#include <string>
#include <map>

#define MAX_PLAYERS 16

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
        //static char * packageBuffer;
        static sf::Packet packet;
        static sf::SocketSelector selector;

        static void sendTCPMessage( const char * buffer, size_t size );
        static void sendUDPMessage( const char * buffer, size_t size );

        static void receivePackets();
        static void parseBuffer();

        static int myPlayerID;
        //static Crystal * myCrystal;

        //static Crystal * crystals[MAX_PLAYERS];
        //static char * options;
        static bool players[MAX_PLAYERS];

        //static void input( sf::Event event );

        static void triggerServerEvent( const char * eventName );
        static void registerServerEvent( const char * eventName, sf::Uint16 id );
        static void requestServerEvents();


        static void requestClientEvent( const char * eventName );
        static void registerClientEvent( const char * eventName, sf::Uint16 id );

    protected:

    private:
        SyncManager();
        static std::map<std::string,sf::Uint16,strless> serverEvents;
        static std::map<std::string,sf::Uint16,strless> clientEvents;
};

#endif // SYNCMANAGER_H
