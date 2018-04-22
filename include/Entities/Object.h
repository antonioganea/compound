#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include "Entity.h"

#include <SFML/Network/Packet.hpp>


class Object : public Entity
{
    public:
        Object();
        virtual ~Object();

        void draw();
        void update( float dt );
        bool isDead();

        sf::Packet setPosition ( const sf::Vector2f& position );
        sf::Packet setPosition ( const float x, const float y );
        sf::Vector2f getPosition();
        void getPosition( float & x, float & y );

        sf::Packet kill();

        sf::Packet setRotation( float rot );
        float getRotation();

        sf::Packet setTextureID( sf::Uint16 id );
        sf::Uint16 getTextureID();

        sf::Packet setVelocity ( const sf::Vector2f& velocity );
        sf::Packet setVelocity ( const float velx, const float vely );
        sf::Vector2f getVelocity();
        void getVelocity( float & velx, float & vely );

        sf::Packet setFriction( float fric );
        float getFriction();

        /** Generates a packet that contains all the object's data */
        sf::Packet generateObjectPacket();

        void setServerID( sf::Uint16 value );
        void setClientID( sf::Uint16 value );
        sf::Uint16 getServerID();
        sf::Uint16 getClientID();

        void setSynced( bool value );
        bool getSynced();

        void setTexture( char * textureName );

    protected:

    private:
        //sf::CircleShape shape;
        sf::Sprite sprite;

        sf::Uint16 clientID, serverID;

        sf::Uint16 textureID;
        float x,y;
        float vx, vy;
        float rotation;
        float friction;
        bool dead;
        bool synced;
};

#endif // OBJECT_H
