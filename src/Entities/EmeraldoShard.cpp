#include "EmeraldoShard.h"

#include "GameRegistry.h"
#include "Display.h"

#include "Mechanics.h"
#include "Emeraldo.h"

#include <stdlib.h>

EmeraldoShard::EmeraldoShard(){
    sf::Texture * texture = GameRegistry::getResource("emeraldo_small.png",ResourceType::Texture).texture;
    shape = new sf::RectangleShape( (sf::Vector2f)texture->getSize() );
    shape->setTexture(texture,false);
    shape->setScale(4.f,4.f);
    lifetime = 0;
    chasing = false;
    rotationalSpeed = rand()%10+30;
    shape->setOrigin(4,4);
}

EmeraldoShard::EmeraldoShard( const sf::Vector2f & _position, const sf::Vector2f & _velocity ){
    EmeraldoShard();

    position = _position;shape->setPosition(position);
    velocity = _velocity;
    lifetime=30;
}

void EmeraldoShard::reset( const sf::Vector2f & _position, const sf::Vector2f & _velocity ){
    position = _position;
    velocity = _velocity;
    lifetime=30;
    shape->setPosition(position);
    chasing = false;
    rotationalSpeed = rand()%10+30;if ( rand()%2 ) rotationalSpeed*=-1;
}

void EmeraldoShard::draw(){

    shape->setRotation( rotationalSpeed + shape->getRotation() );

    Display::window->draw(*shape);
}

void EmeraldoShard::update(float dt)
{
    Mechanics::applyFriction(velocity,0.1f);
    position+=velocity;
    shape->setPosition(position);

    if (chasing)
        chase();
    if ( lifetime ){
        lifetime--;
        if ( !lifetime ){
            chase();
        }
    }
}

void EmeraldoShard::chase(){
    if ( Mechanics::checkEpsilon(target->position,position,0.5f) ){
        chasing = false;
        velocity *= 0.f;
        return;
    }
    velocity = target->position-position;
    Mechanics::applyMaxSpeed(velocity,15.f);
    chasing = true;
}

bool EmeraldoShard::isDead()
{
    return !chasing && !lifetime;
}
