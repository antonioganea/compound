#ifndef RUBIELASER_H
#define RUBIELASER_H

#include "Entity.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include "Crystal.h"

class Rubie;

class RubieLaser : public Entity
{
    public:
        static const float laserLength;

        RubieLaser();
        RubieLaser( const sf::Vector2f & position, const sf::Vector2f & velocity );
        virtual ~RubieLaser();

        void reset( const sf::Vector2f & position, const sf::Vector2f & velocity );

        void draw();
        void update( float dt );

        bool isDead();
        bool dead;

        sf::RectangleShape * shape;
        sf::Vector2f position;
        sf::Vector2f velocity;
        int lifetime;

        bool checkCollision( Crystal * crystal );
    protected:
    private:
        int animationTimer;
};

#endif // RUBIELASER_H
