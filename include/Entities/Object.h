#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics/CircleShape.hpp>
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

        void setPosition ( const sf::Vector2f& position );
        void setPosition ( const float x, const float y );
        sf::Vector2f getPosition();
        void getPosition( float & x, float & y );

        void kill();

        void setRotation( float rot );
        float getRotation();

        void setTextureID( sf::Uint16 id );
        sf::Uint16 getTextureID();

        void setVelocity ( const float velx, const float vely );
        sf::Vector2f getVelocity();
        void getVelocity( float & velx, float & vely );

        void setFriction( float fric );
        float getFriction();

        /** Generates a packet that contains all the object's data */
        sf::Packet generateObjectPacket();

        void setServerID( sf::Uint16 value );
        void setClientID( sf::Uint16 value );
        sf::Uint16 getServerID();
        sf::Uint16 getClientID();

        void setSynced( bool value );
        bool getSynced();

    protected:

    private:
        sf::CircleShape shape;

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
