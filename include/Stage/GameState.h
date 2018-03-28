#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>

#include "Stage.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class GameState : public Stage
{
    public:
        GameState();
        virtual ~GameState();

        void addEntity(Entity * entity);
        void update( float dt );
        void input( const sf::Event & event );
        void draw();
        sf::CircleShape circle;
        sf::RectangleShape arena;
    protected:

    private:
        std::vector<Entity*> m_entities;
        std::vector<Entity*> ps_entities;
};

#endif // GAMESTATE_H
