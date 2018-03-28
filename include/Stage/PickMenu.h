#ifndef PICKMENU_H
#define PICKMENU_H

#include <vector>

#include "Stage.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "GuiButton.h"

class PickMenu : public Stage
{
    public:
        PickMenu();
        virtual ~PickMenu();

        void addEntity(Entity * entity);
        void update( float dt );
        void input( const sf::Event & event );
        void draw();

        sf::RectangleShape arena;
    protected:

    private:
        std::vector<GuiButton*> m_buttons;
};

#endif // PICKMENU_H
