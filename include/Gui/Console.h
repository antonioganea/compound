#ifndef CONSOLE_H
#define CONSOLE_H

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class Console
{
    public:
        Console();
        virtual ~Console();

        void setVisible( bool visible );
        bool input( sf::Event event );

        void draw();

        void pushTextElement( char * buffer );

    protected:

    private:
        sf::RectangleShape background;
        sf::CircleShape triangle;
        sf::Text currentInput;
        sf::Text label;
        float coverage = 0.3f;
        bool visible = false;
        float width, height;
};

#endif // CONSOLE_H
