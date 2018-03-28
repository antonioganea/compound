#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "Entity.h"
#include "Crystal.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class HealthBar : public Entity
{
    public:
        HealthBar();
        virtual ~HealthBar();
        void draw();
        void update( float dt );
        bool isDead();
        sf::RectangleShape shape;
        sf::RectangleShape filler;
        Crystal * targetCrystal;
        void setCrystal ( Crystal * crystal );
        static const sf::Vector2f offset;
    protected:
    private:
};

#endif // HEALTHBAR_H
