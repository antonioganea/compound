#ifndef GUIEDITFIELD_H
#define GUIEDITFIELD_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include <string>

class GuiEditField
{
    public:
        GuiEditField( std::string defaultString, int x, int y, int w, int h );
        void highlight( bool hightlightOn );
        void setPosition( float x, float y );
        void setPosition( sf::Vector2f position );
        void setString( std::string str );
        char * getString();
        void checkType( sf::Event::KeyEvent keyEvent );
        bool checkClick( sf::Event::MouseButtonEvent buttonEvent );
        bool checkHover( sf::Event::MouseMoveEvent moveEvent );
        sf::RectangleShape shape;
        sf::Text text;
        void draw();

        char stringBuffer[16];
    protected:
    private:
};

#endif // GUIEDITFIELD_H
