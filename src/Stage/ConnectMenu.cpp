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
    button->setPosition(WINDOW_WIDTH/2,680);
    m_buttons.push_back(button);

    button = new GuiButton(6);
    button->setPosition(WINDOW_WIDTH/2,540);
    m_buttons.push_back(button);

    ipField = new GuiEditField( "127.0.0.1", WINDOW_WIDTH/2, 250, 400, 80 );
    ipField->ipField = true;
    nameField = new GuiEditField( "Player", WINDOW_WIDTH/2, 350, 400, 80 );
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
            ipField->checkHover(event.mouseMove);
            nameField->checkHover(event.mouseMove);
            break;
        case sf::Event::MouseButtonReleased:
            ipField->checkClick(event.mouseButton);
            nameField->checkClick(event.mouseButton);
            break;
        case sf::Event::KeyPressed:
            ipField->checkType(event.key);
            nameField->checkType(event.key);
            break;
        case sf::Event::TextEntered:
            nameField->checkText(event.text);
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
    ipField->draw();
    nameField->draw();
}
