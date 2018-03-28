#include "Particle.h"

#include "GameRegistry.h"
#include "Display.h"

#include "Mechanics.h"
#include <math.h>

Particle::Particle(){
    lifetime = 0;
}

Particle::Particle( const sf::Texture * texture ){
    Particle();
    setTexture( texture );
}

void Particle::reset( const sf::Vector2f & _position, const sf::Vector2f & _velocity, const int & _lifetime ){
    position = _position;
    velocity = _velocity;
    lifetime = _lifetime;
    shape->setPosition(position);
    rotationalSpeed = rand()%5+15;if ( rand()%2 ) rotationalSpeed*=-1;
}

Particle::~Particle(){
    //dtor
}

void Particle::draw(){
    Display::window->draw(*shape);
}

void Particle::setTexture( const sf::Texture * texture ){
    shape = new sf::RectangleShape( (sf::Vector2f)texture->getSize() );
    shape->setTexture( texture, false );
    shape->setOrigin(4.f,4.f);
    shape->setScale( 3.0f, 3.0f );
}


void Particle::update(float dt){
    Mechanics::applyFriction(velocity,0.1f);
    position+=velocity;

    shape->setRotation( rotationalSpeed + shape->getRotation() );

    shape->setPosition(position);
    if (lifetime)
        lifetime--;
}

bool Particle::isDead(){
    return !lifetime;
}

bool Particle::checkCollision( Crystal * crystal ){
    if ( !crystal->isCollidable() )
        return false;

    sf::Vector2f target = crystal->getPosition();

    float dist = sqrt(
    (target.x - position.x)*(target.x - position.x) +
    (target.y - position.y)*(target.y - position.y) );

    if ( crystal->getRadius() + 12.f >= dist )
        return true;
    return false;
}
