#ifndef CRYSTAL_H
#define CRYSTAL_H

#include "Entity.h"
#include <SFML/Window/Event.hpp>

class Crystal : public Entity
{
    public:
        virtual void draw() = 0;
        virtual void update( float dt ) = 0;
        virtual bool isDead() = 0;

        virtual void input( const sf::Event & event ) = 0;

        virtual void setPosition ( const sf::Vector2f& position ) = 0;

        virtual void setSyncable( bool syncable ) = 0;

        virtual void move() = 0;
        virtual void attack() = 0;
        virtual void ultimate() = 0;

        virtual void pushCrystal( const sf::Vector2f& velocity ) = 0;

        virtual sf::Vector2f getPosition() = 0;
        virtual float getRadius() = 0;
        virtual bool isCollidable() = 0;

        virtual int getHP() = 0;
        virtual void setHP( int newHP ) = 0;

        virtual void markDead() = 0;

        virtual void setSyncer( int _syncer ) = 0;
        virtual int getSyncer() = 0;
    protected:
    private:
};

#endif // CRYSTAL_H
