#include "GuiLabel.h"

#include "Display.h"
#include <SFML/Graphics/Texture.hpp>

GuiLabel::GuiLabel( sf::Texture * texture ){
    shape.setSize( (sf::Vector2f)texture->getSize() );
    shape.setOrigin( (sf::Vector2f)texture->getSize() / 2.f );
    shape.setTexture( texture );
}

void GuiLabel::setPosition(float x, float y){
    shape.setPosition( x, y );
}

void GuiLabel::setPosition(sf::Vector2f position){
    shape.setPosition( position );
}

void GuiLabel::draw(){
    Display::window->draw(shape);
}
