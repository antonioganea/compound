#ifndef CONSOLE_H
#define CONSOLE_H

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class Console
{
    public:

        //virtual ~Console();

        static void init();
        static void setVisible( bool visible );
        static bool getVisible();
        static bool input( sf::Event event );

        static void draw();


        static void pushTextElement ( const char * buffer );

    protected:

    private:
        Console();
        static void generateTextElements();
        static sf::RectangleShape background;
        static sf::CircleShape triangle;
        static sf::Text currentInput;
        static sf::Text label;
        static float coverage;
        static bool visible;
        static float width, height;
};

#endif // CONSOLE_H
