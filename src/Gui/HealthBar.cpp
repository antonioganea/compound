#include "HealthBar.h"
#include "Display.h"

const sf::Vector2f HealthBar::offset(-16,-40);

HealthBar::HealthBar(){
    targetCrystal = 0;
    shape.setSize( sf::Vector2f(32, 4) );
    shape.setFillColor(sf::Color::Red);
    shape.setOutlineColor( sf::Color::Black );
    shape.setOutlineThickness(1.f);

    filler.setSize( sf::Vector2f(16, 4) );
    filler.setFillColor(sf::Color::Green);
}

HealthBar::~HealthBar(){
    //dtor
}

void HealthBar::draw(){
    Display::window->draw(shape);
    Display::window->draw(filler);
}

void HealthBar::update(float dt){
    if ( targetCrystal ){
        shape.setPosition( targetCrystal->getPosition() + HealthBar::offset );
        filler.setPosition( targetCrystal->getPosition() + HealthBar::offset );

        int displayHP = targetCrystal->getHP();
        displayHP = (displayHP*32)/100;
        filler.setSize( sf::Vector2f( displayHP, 4 ));
    }
}

bool HealthBar::isDead(){
    if ( targetCrystal ){
        return targetCrystal->isDead();
    }
    return true;
}

void HealthBar::setCrystal(Crystal* crystal){
    targetCrystal = crystal;
}
