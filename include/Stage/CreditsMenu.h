#ifndef CREDITSMENU_H
#define CREDITSMENU_H

#include <vector>

#include "Stage.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "GuiButton.h"
#include "GuiLabel.h"

class CreditsMenu : public Stage
{
    public:
        CreditsMenu();
        virtual ~CreditsMenu();

        void addEntity(Entity * entity);
        void update( float dt );
        void input( const sf::Event & event );
        void draw();

        sf::RectangleShape arena;
    protected:

    private:
        std::vector<GuiLabel*> m_labels;
        std::vector<GuiButton*> m_buttons;
};

#endif // CREDITSMENU_H
