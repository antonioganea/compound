#include "Object.h"

#include "Display.h"

Object::Object(){
    //ctor
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(0,0);
    shape.setPointCount(3);
    shape.setRadius(30);

    shape.setOrigin(30,30);

    textureID = 0;
    x = y = vx = vy = rotation = friction = 0.f;
    dead = false;
}

Object::~Object(){
    //dtor
}

void Object::draw(){
    Display::window->draw(shape);
}

void Object::update(float dt){
    // Update position
    x += vx;
    y += vy;

    shape.setPosition(x,y);

    // Update velocity
    vx -= vx*friction;
    vy -= vy*friction;
}



bool Object::isDead(){
    return dead;
}

void Object::setPosition(const sf::Vector2f& position)
{
    shape.setPosition(position);
    x = position.x;
    y = position.y;
}

void Object::setPosition(const float _x, const float _y){
    x = _x;
    y = _y;
    shape.setPosition(x,y);
}

sf::Vector2f Object::getPosition(){
    return shape.getPosition();
}

void Object::getPosition(float& _x, float& _y)
{
    _x = x;
    _y = y;
}

void Object::kill(){
    dead = true;
}

void Object::setRotation(float rot){
    rotation = rot;
    shape.setRotation(rot);
}

float Object::getRotation(){
    return rotation;
}

void Object::setTextureID(sf::Uint16 id){
    textureID = id;

    //Temporary checking it with colors instead of textures
    sf::Color color;
    switch ( id ){
    case 0:
        color = sf::Color::Green;
        break;
    case 1:
        color = sf::Color::Red;
        break;
    case 2:
        color = sf::Color::Blue;
        break;
    case 3:
        color = sf::Color::Black;
        break;
    case 4:
        color = sf::Color::White;
        break;
    case 5:
        color = sf::Color::Yellow;
        break;
    }
    shape.setFillColor(color);
}

sf::Uint16 Object::getTextureID(){
    return textureID;
}

void Object::setVelocity(const float velx, const float vely){
    vx = velx;
    vy = vely;
}

sf::Vector2f Object::getVelocity(){
    sf::Vector2f velocity;
    velocity.x = vx;
    velocity.y = vy;
    return velocity;
}

void Object::getVelocity(float& velx, float& vely){
    velx = vx;
    vely = vy;
}

void Object::setFriction(float fric){
    friction = fric;
}

float Object::getFriction(){
    return friction;
}
