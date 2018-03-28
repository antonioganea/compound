#ifndef PARTICLE_H
#define PARTICLE_H

#include "Entity.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include "Crystal.h"

class Particle : public Entity
{
    public:
        Particle();
        Particle( const sf::Texture * texture );
        virtual ~Particle();

        void reset( const sf::Vector2f & position, const sf::Vector2f & velocity, const int & _lifetime );

        void draw();
        void update( float dt );

        bool checkCollision( Crystal * crystal );

        void setTexture ( const sf::Texture * texture );

        bool isDead();
        sf::RectangleShape * shape;
        sf::Vector2f position;
        sf::Vector2f velocity;
        int lifetime;
        int rotationalSpeed;
    protected:
    private:
};

#endif // PARTICLE_H
