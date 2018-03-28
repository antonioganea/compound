#ifndef SAPHEER_H
#define SAPHEER_H

#include "Crystal.h"
#include "RubieLaser.h"
#include <SFML/Graphics/Sprite.hpp>
#include "Particle.h"
#include "HealthBar.h"

class Sapheer : public Crystal{
    public:
        Sapheer();
        void draw();
        void update( float dt );

        bool isDead();
        bool dead;

        void input( const sf::Event & event );

        sf::Vector2f getPosition();
        void setPosition ( const sf::Vector2f& position );

        void setSyncable( bool syncable );

        void move();
        void attack();
        void ultimate();

        void pushCrystal( const sf::Vector2f& velocity );

        bool syncable;

        float getRadius();
        bool isCollidable();
        int hp;
        int getHP();
        void setHP( int newHP );

        void markDead();

        int syncer;
        void setSyncer( int syncer );
        int getSyncer();

        HealthBar healthBar;
    protected:
    private:
        const static float acceleration;
        const static float friction;

        sf::Sprite sprite;

        sf::RectangleShape * shield;
        void throwShield();

        Particle * shootParticles;
        void shoot();

        sf::Vector2f velocity;
        int w,a,s,d;
        int cooldown, animationTimer, shieldTimer;
};

#endif // SAPHEER_H
