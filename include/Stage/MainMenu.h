#ifndef MAINMENU_H
#define MAINMENU_H

#include <vector>

#include "Stage.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "GuiButton.h"

class MainMenu : public Stage
{
    public:
        MainMenu();
        virtual ~MainMenu();

        void addEntity(Entity * entity);
        void update( float dt );
        void input( const sf::Event & event );
        void draw();

        sf::RectangleShape arena;
    protected:

    private:
        std::vector<GuiButton*> m_buttons;
};

#endif // MAINMENU_H
