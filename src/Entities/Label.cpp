#include "Label.h"

#include "Display.h"
#include "GameRegistry.h"

Label::Label(){
    //ctor

    text.setFont( *((GameRegistry::getResource("ARCADECLASSIC.TTF",ResourceType::Font )).font) );
    text.setCharacterSize(16);
    text.setString("<EMPTY>");
    text.setFillColor(sf::Color::Magenta);
    text.setPosition(20,20);

    dead = false;

}

Label::~Label(){
    //dtor
}

void Label::draw(){
    Display::window->draw(text);
}

void Label::update(float dt){

}

bool Label::isDead(){
    return dead;
}

void Label::setFont(sf::Font f){

}

void Label::setPosition(int x, int y){
    text.setPosition(x,y);
}

void Label::setSize(int size){
    text.setCharacterSize(size);
}

void Label::setColor(int id){
    sf::Color color;
    switch (id){
        case 0:
            color = sf::Color::Black;
            break;
        case 1:
            color = sf::Color::Green;
            break;
        case 2:
            color = sf::Color::Red;
            break;
        case 3:
            color = sf::Color::Yellow;
            break;
        case 4:
            color = sf::Color::Blue;
            break;
    }
    text.setFillColor(color);
}

void Label::destroy(){
    dead = true;
}

void Label::setText(char* str){
    text.setString(str);
}
