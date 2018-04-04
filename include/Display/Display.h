#ifndef DISPLAY_H
#define DISPLAY_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include "Crystal.h"

class Display
{
    public:
        static void init();
        static void focusOn( Crystal * crystal );
        static void resetView();
        static sf::RenderWindow * window;
        static sf::View view;
        static sf::View GuiView;
    protected:

    private:
        Display();
};

#endif // DISPLAY_H
