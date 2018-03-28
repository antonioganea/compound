#ifndef EMERALDOSHARD_H
#define EMERALDOSHARD_H

#include "Entity.h"
#include <SFML/Graphics/RectangleShape.hpp>

class Emeraldo;

class EmeraldoShard : public Entity
{
    public:
        EmeraldoShard();
        EmeraldoShard( const sf::Vector2f & position, const sf::Vector2f & velocity );

        void reset( const sf::Vector2f & position, const sf::Vector2f & velocity );

        void draw();
        void update( float dt );

        void chase();

        bool isDead();

        EmeraldoShard * target;
        sf::Vector2f position;
    protected:
    private:
        sf::RectangleShape * shape;

        sf::Vector2f velocity;

        int lifetime;
        bool chasing;
        float rotationalSpeed;
};

#endif // EMERALDOSHARD_H
