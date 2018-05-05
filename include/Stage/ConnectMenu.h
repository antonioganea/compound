#ifndef CONNECTMENU_H
#define CONNECTMENU_H

#include <vector>

#include "Stage.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "GuiButton.h"
#include "GuiLabel.h"
#include "GuiEditField.h"

class ConnectMenu : public Stage
{
    public:
        ConnectMenu();
        virtual ~ConnectMenu();

        void addEntity(Entity * entity);
        void update( float dt );
        void input( const sf::Event & event );
        void draw();

        sf::RectangleShape arena;
        GuiEditField * ipField;
        GuiEditField * nameField;
    protected:

    private:
        std::vector<GuiLabel*> m_labels;
        std::vector<GuiButton*> m_buttons;

};

#endif // CONNECTMENU_H
