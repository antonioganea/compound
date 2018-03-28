#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>

class GuiButton
{
    public:
        int functionality;
        GuiButton( int functionality );
        void highlight( bool hightlightOn );
        void setNext( GuiButton * button );
        void setLast( GuiButton * button );
        void setPosition( float x, float y );
        void setPosition( sf::Vector2f position );
        bool checkClick( sf::Event::MouseButtonEvent buttonEvent );
        bool checkHover( sf::Event::MouseMoveEvent moveEvent );
        GuiButton * next = nullptr;
        GuiButton * last = nullptr;
        sf::RectangleShape shape;
        void draw();
    protected:
    private:
};

#endif // GUIBUTTON_H
