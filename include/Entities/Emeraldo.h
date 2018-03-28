#ifndef EMERALDO_H
#define EMERALDO_H

#include "Crystal.h"
#include <SFML/Graphics/Sprite.hpp>
#include "EmeraldoShard.h"
#include "Particle.h"
#include "HealthBar.h"

class Emeraldo : public Crystal{
    public:
        Emeraldo();
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

        void disperse();
        int w,a,s,d;

        bool syncable;

        float getRadius();
        bool isCollidable();
        int hp;
        int getHP();
        void setHP( int newHP );

        void markDead();

        void setSyncer( int syncer );
        int getSyncer();

        int syncer;

        HealthBar healthBar;
    protected:
    private:
        const static float acceleration;
        const static float friction;
        const static int lookup[9];
        const static int abilityCooldown;

        sf::Sprite sprite;

        EmeraldoShard * dispersionParticles;

        Particle * shootParticles;
        void shoot();

        sf::Vector2f velocity;

        bool inDispersion, visible;
        int cooldown, animationTimer, dispersionParticleNo;
};

#endif // EMERALDO_H
