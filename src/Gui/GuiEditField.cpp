#include "GuiEditField.h"

#include "Display.h"
#include "GameRegistry.h"
#include <iostream>

#include "StageManager.h"

#include <string.h>

GuiEditField::GuiEditField( std::string defaultString, int x, int y, int w, int h ){
    text.setFont(*GameRegistry::getResource("dpcomic.ttf",ResourceType::Font).font);
    text.setPosition(x,y);
    text.setCharacterSize(48);
    text.setColor(sf::Color::Black);
    setString( defaultString );

    shape.setSize(sf::Vector2f( w, h ));
    shape.setOrigin(sf::Vector2f( w, h )/2.f);
    shape.setFillColor(sf::Color::White);
    shape.setPosition(x,y);

    strcpy(stringBuffer,defaultString.c_str());

    highlight(false);
}

void GuiEditField::setString( std::string str ){
    text.setString( str );
    text.setOrigin( text.getGlobalBounds().width/2.f, text.getGlobalBounds().height/2.f );
}

char * GuiEditField::getString(){
    return stringBuffer;
}

void GuiEditField::checkType( sf::Event::KeyEvent keyEvent ){
    int len = strlen(stringBuffer);
    if ( keyEvent.code == sf::Keyboard::BackSpace ){
        if ( len > 0 )
            stringBuffer[len-1] = 0;
    }else if ( keyEvent.code >= sf::Keyboard::Num0 && keyEvent.code <= sf::Keyboard::Num9 ){
        if ( len < 15 )
            stringBuffer[len] = keyEvent.code - sf::Keyboard::Num0 + '0';
    }else if ( keyEvent.code == sf::Keyboard::Period ){
        if ( len < 15 )
            stringBuffer[len] = '.';
    }

    setString(stringBuffer);
}

void GuiEditField::highlight( bool hightlightOn ){
    if ( hightlightOn ){
        shape.setFillColor(sf::Color::Green);
    }
    else{
        shape.setFillColor(sf::Color::White);
    }
}

void GuiEditField::setPosition(float x, float y){
    shape.setPosition(x,y);
}

void GuiEditField::setPosition(sf::Vector2f position){
    shape.setPosition(position);
}

bool GuiEditField::checkClick(sf::Event::MouseButtonEvent buttonEvent){
    highlight(false);
    if ( shape.getGlobalBounds().contains(buttonEvent.x,buttonEvent.y) ){
        highlight(true);
        return true;
    }
    return false;
}

bool GuiEditField::checkHover(sf::Event::MouseMoveEvent moveEvent){
    return false;
}

void GuiEditField::draw(){
    Display::window->draw(shape);
    Display::window->draw(text);
}
