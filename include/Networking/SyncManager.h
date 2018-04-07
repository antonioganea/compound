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

        static sf::TcpSocket tcpSocket;
        static sf::UdpSocket udpSocket;
        static sf::IpAddress address;
        static short unsigned int serverPort;
        static short unsigned int localUDPPort;
        static bool connected;
        static char * packageBuffer;
        static sf::SocketSelector selector;

        static void sendTCPMessage( const char * buffer, size_t size );
        static void sendUDPMessage( const char * buffer, size_t size );

        //static void sendCrystalType ( int type );
        //static void sendPosition( const sf::Vector2f& position );
        //static void sendShoot( const sf::Vector2f& velocity );
        //static void sendUltimate();
        //static void sendUltimate( char options );
        //static void sendUltimate( float options );
        //static void sendLaserCollision( int player );
        //static void sendParticleCollision( int player );
        //static void sendShieldCollision( int player );

        static void receivePackets();
        static void parseBuffer( std::size_t received );

        static int myPlayerID;
        //static Crystal * myCrystal;

        //static Crystal * crystals[MAX_PLAYERS];
        //static char * options;
        static bool players[MAX_PLAYERS];

        //static void input( sf::Event event );

        static void triggerServerEvent( const char * eventName );
        static void registerServerEvent( const char * eventName, int id );

    protected:

    private:
        SyncManager();
        static std::map<std::string,int,strless> serverEvents;
};

#endif // SYNCMANAGER_H
