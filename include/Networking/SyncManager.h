#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#include <SFML/Network.hpp>
#include "Crystal.h"

#define MAX_PLAYERS 16

class SyncManager
{
    public:
        static void init();
        static void connectToServer( const sf::IpAddress& address );
        static void sendCrystalType ( int type );
        static void sendPosition( const sf::Vector2f& position );
        static void sendShoot( const sf::Vector2f& velocity );
        static void sendUltimate();
        static void sendUltimate( char options );
        static void sendUltimate( float options );
        static void sendLaserCollision( int player );
        static void sendParticleCollision( int player );
        static void sendShieldCollision( int player );

        static void receivePackets();
        static void parseBuffer( std::size_t received );

        static int myPlayerID;
        static Crystal * myCrystal;

        static Crystal * crystals[MAX_PLAYERS];
        static char * options;
        static bool players[MAX_PLAYERS];

        static sf::TcpSocket tcpSocket;
        static sf::UdpSocket udpSocket;
        static sf::IpAddress address;
        static short unsigned int serverPort;
        static short unsigned int localUDPPort;
        static bool connected;

        static void input( sf::Event event );

        static char * packageBuffer;

        static sf::SocketSelector selector;
    protected:

    private:
        SyncManager();
};

#endif // SYNCMANAGER_H
