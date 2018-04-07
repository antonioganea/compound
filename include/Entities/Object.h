#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include "Entity.h"


class Object : public Entity
{
    public:
        Object();
        virtual ~Object();
        void draw();
        void update( float dt );
        bool isDead();
        void setPosition ( const sf::Vector2f& position );
        void setPosition ( const int x, const int y );
        sf::Vector2f getPosition();

    protected:

    private:
        sf::CircleShape shape;
};

#endif // OBJECT_H
