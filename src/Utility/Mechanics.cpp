#include "Mechanics.h"

#define _USE_MATH_DEFINES
#include <cmath>

void Mechanics::applyFriction( sf::Vector2f& velocity, const float& friction ){
    sf::Vector2f counterVelocity;
    counterVelocity = velocity * friction;
    velocity -= counterVelocity;
}

void Mechanics::applyMaxSpeed( sf::Vector2f& velocity, const float& maxSpeed ){
    //Speed limits
    sf::Vector2f maxVector;
    float angle = atan2( velocity.y, velocity.x );

    maxVector.x = cos(angle)*maxSpeed;
    maxVector.y = sin(angle)*maxSpeed;

    if ( velocity.x*velocity.x + velocity.y*velocity.y >
        maxVector.x*maxVector.x + maxVector.y*maxVector.y )
        velocity = maxVector;
}

void Mechanics::applyAcceleration( sf::Vector2f& velocity, const float& xDir, const float& yDir, const float& acceleration ){
    velocity.x += xDir*acceleration;
    velocity.y += yDir*acceleration;
}

bool Mechanics::checkEpsilon(const sf::Vector2f& v1, const sf::Vector2f& v2, const float& epsilon)
{
    sf::Vector2f test = v2-v1;
    if ( std::abs(test.x) <= epsilon && std::abs(test.y) <= epsilon )
        return true;
    return false;
}

float Mechanics::getSpeed( const sf::Vector2f& velocity ){
    return sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
}
