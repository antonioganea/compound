#include "RubieLaser.h"

#include "GameRegistry.h"
#include "Display.h"

#include "Mechanics.h"
#include "Rubie.h"

#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "SyncManager.h"

#include <iostream>

const float RubieLaser::laserLength = 60.f;

RubieLaser::RubieLaser(){
    sf::Texture * texture = GameRegistry::getResource("laser.png",ResourceType::Texture).texture;
    shape = new sf::RectangleShape( sf::Vector2f(30,8) );
    shape->setTexture(texture,false);
    shape->setTextureRect(sf::IntRect(sf::Vector2i(0,0),sf::Vector2i(30,8)));
    shape->setScale(2.f,2.f);
    lifetime = 0;
    shape->setOrigin(15,4);
    animationTimer = 0;
    dead = false;
}

RubieLaser::RubieLaser( const sf::Vector2f & _position, const sf::Vector2f & _velocity ){
    RubieLaser();

    position = _position;shape->setPosition(position);
    velocity = _velocity;
    lifetime=30;
}

void RubieLaser::reset( const sf::Vector2f & _position, const sf::Vector2f & _velocity ){
    position = _position;
    velocity = _velocity;
    lifetime=30;
    shape->setPosition(position);
    animationTimer = 0;
    dead = false;

    float angle = atan2( velocity.y, velocity.x );
    shape->setRotation(angle*180.f/M_PI);
}

RubieLaser::~RubieLaser()
{
    //dtor
}

void RubieLaser::draw(){
    Display::window->draw(*shape);
}

void RubieLaser::update(float dt)
{
    Mechanics::applyFriction(velocity,0.05f);
    position+=velocity;

    if ( animationTimer < 8 ){
        shape->setTextureRect(sf::IntRect(sf::Vector2i(animationTimer/2*30,0),sf::Vector2i(30,8)));
        animationTimer++;
    }

    shape->setPosition(position);

    if (lifetime)
        lifetime--;
}

bool RubieLaser::isDead()
{
    return dead | (Mechanics::getSpeed(velocity) < 5.f);
}

bool RubieLaser::checkCollision( Crystal * crystal ){
    if ( !crystal->isCollidable() )
        return false;

    sf::Vector2f D; // direction vector

    float angle = atan2( velocity.y, velocity.x );
    D.x = cos ( angle );
    D.y = sin ( angle );

    sf::Vector2f C = crystal->getPosition();

    sf::Vector2f A = shape->getPosition();
    A -= D*30.f;

    float t = D.x * ( C.x - A.x ) + D.y * ( C.y - A.y );

    sf::Vector2f E;

    E.x = t*D.x+A.x;
    E.y = t*D.y+A.y;

    if ( sqrt( (E.x-A.x)*(E.x-A.x)+(E.y-A.y)*(E.y-A.y) ) > laserLength/2.f )
        return false;

    float LEC = sqrt( (E.x-C.x)*(E.x-C.x)+(E.y-C.y)*(E.y-C.y) );

    if ( LEC < crystal->getRadius() ){
        return true;
    }
    return false;
}
