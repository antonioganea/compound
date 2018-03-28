#include "ConnectMenu.h"

#include <iostream>

#include "GameRegistry.h"
#include "Display.h"

#include "GuiButton.h"
#include "Config.h"

ConnectMenu::ConnectMenu()
{
    std::cout << "Connect Menu created!\n";

    sf::Texture * arenaTexture = GameRegistry::getResource("arena.png",ResourceType::Texture).texture;
    arena.setSize( sf::Vector2f(WINDOW_WIDTH,WINDOW_HEIGHT) );
    arena.setTexture(arenaTexture);

    GuiButton * button = new GuiButton(5); // back
    button->setPosition(960,680);
    m_buttons.push_back(button);

    button = new GuiButton(6);
    button->setPosition(960,540);
    m_buttons.push_back(button);

    field = new GuiEditField( "127.0.0.1", 960, 300, 300, 80 );
}

ConnectMenu::~ConnectMenu(){
    //dtor
}

void ConnectMenu::addEntity( Entity * entity ){
}

void ConnectMenu::update(float dt){
}

void ConnectMenu::input( const sf::Event & event ){
    for ( std::vector<GuiButton*>::iterator it = m_buttons.begin(); it != m_buttons.end(); it++ ){
        switch( event.type ){
            case sf::Event::MouseMoved:
                (*it)->checkHover(event.mouseMove);
                break;
            case sf::Event::MouseButtonReleased:
                (*it)->checkClick(event.mouseButton);
                break;
            default:
                break;
        }
    }
    switch( event.type ){
        case sf::Event::MouseMoved:
            field->checkHover(event.mouseMove);
            break;
        case sf::Event::MouseButtonReleased:
            field->checkClick(event.mouseButton);
            break;
        case sf::Event::KeyPressed:
            field->checkType(event.key);
            break;
        default:
            break;
    }
}

void ConnectMenu::draw()
{
    Display::window->draw(arena);
    for ( std::vector<GuiButton*>::iterator it = m_buttons.begin(); it != m_buttons.end(); it++ ){
        (*it)->draw();
    }
    for ( std::vector<GuiLabel*>::iterator it = m_labels.begin(); it != m_labels.end(); it++ ){
        (*it)->draw();
    }
    field->draw();
}
