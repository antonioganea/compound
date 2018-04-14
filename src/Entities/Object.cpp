#include "Object.h"

#include "Display.h"

Object::Object(){
    //ctor
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(0,0);
    shape.setPointCount(30);
    shape.setRadius(30);
}

Object::~Object(){
    //dtor
}

void Object::draw(){
    Display::window->draw(shape);
}

void Object::update(float dt){

}

bool Object::isDead(){
    return false;
}

void Object::setPosition(const sf::Vector2f& position)
{
    shape.setPosition(position);
}

void Object::setPosition(const int x, const int y)
{
    shape.setPosition(x,y);
}


sf::Vector2f Object::getPosition()
{
    return shape.getPosition();
}
