#include "Object.h"

#include "Display.h"

#include "Events.h"

Object::Object(){
    //ctor
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(0,0);
    shape.setPointCount(3);
    shape.setRadius(30);

    shape.setOrigin(30,30);

    serverID = clientID = 0;
    textureID = 0;
    x = y = vx = vy = rotation = friction = 0.f;
    dead = false;
    synced = false;
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

sf::Packet Object::setPosition(const sf::Vector2f& position)
{
    shape.setPosition(position);
    x = position.x;
    y = position.y;

    sf::Packet newPacket;
    newPacket << SHARED_POSITION << serverID << x << y;
    return newPacket;
}

sf::Packet Object::setPosition(const float _x, const float _y){
    x = _x;
    y = _y;
    shape.setPosition(x,y);

    sf::Packet newPacket;
    newPacket << SHARED_POSITION << serverID << x << y;
    return newPacket;
}

sf::Vector2f Object::getPosition(){
    return shape.getPosition();
}

void Object::getPosition(float& _x, float& _y)
{
    _x = x;
    _y = y;
}

sf::Packet Object::kill(){
    dead = true;

    sf::Packet newPacket;
    newPacket << SHARED_KILL << serverID;
    return newPacket;
}

sf::Packet Object::setRotation(float rot){
    rotation = rot;
    shape.setRotation(rot);

    sf::Packet newPacket;
    newPacket << SHARED_ROTATION << serverID << rot;
    return newPacket;
}

float Object::getRotation(){
    return rotation;
}

sf::Packet Object::setTextureID(sf::Uint16 id){
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

    sf::Packet newPacket;
    newPacket << SHARED_TEXTUREID << serverID << textureID;
    return newPacket;
}

sf::Uint16 Object::getTextureID(){
    return textureID;
}

sf::Packet Object::setVelocity( const sf::Vector2f& velocity ){
    vx = velocity.x;
    vy = velocity.y;

    sf::Packet newPacket;
    newPacket << SHARED_VELOCITY << serverID << vx << vy;
    return newPacket;
}

sf::Packet Object::setVelocity(const float velx, const float vely){
    vx = velx;
    vy = vely;

    sf::Packet newPacket;
    newPacket << SHARED_VELOCITY << serverID << vx << vy;
    return newPacket;
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

sf::Packet Object::setFriction(float fric){
    friction = fric;

    sf::Packet newPacket;
    newPacket << SHARED_FRICTION << serverID << friction;
    return newPacket;
}

float Object::getFriction(){
    return friction;
}

sf::Packet Object::generateObjectPacket(){
    sf::Packet newPacket;
    newPacket << S_REQUEST_REGISTER_OBJECT << clientID << x << y << vx << vy << friction << rotation << textureID;
    return newPacket;
}

void Object::setServerID(sf::Uint16 value){
    serverID = value;
}

void Object::setClientID(sf::Uint16 value){
    clientID = value;
}

sf::Uint16 Object::getServerID(){
    return serverID;
}

sf::Uint16 Object::getClientID(){
    return clientID;
}

void Object::setSynced(bool value){
    synced = value;
}

bool Object::getSynced(){
    return synced;
}

