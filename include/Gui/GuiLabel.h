#ifndef GUILABEL_H
#define GUILABEL_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>

class GuiLabel
{
    public:
        GuiLabel( sf::Texture * texture );
        void setPosition( float x, float y );
        void setPosition( sf::Vector2f position );
        sf::RectangleShape shape;
        void draw();
    protected:
    private:
};

#endif // GUILABEL_H
