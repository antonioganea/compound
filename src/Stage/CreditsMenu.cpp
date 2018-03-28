#include "CreditsMenu.h"

#include <iostream>

#include "GameRegistry.h"
#include "Display.h"

#include "GuiButton.h"
#include "Config.h"

CreditsMenu::CreditsMenu()
{
    std::cout << "Credits Menu created!\n";

    sf::Texture * arenaTexture = GameRegistry::getResource("arena.png",ResourceType::Texture).texture;
    arena.setSize( sf::Vector2f(WINDOW_WIDTH,WINDOW_HEIGHT) );
    arena.setTexture(arenaTexture);

    GuiButton * button = new GuiButton(5);
    button->setPosition(160,80);
    m_buttons.push_back(button);
}

CreditsMenu::~CreditsMenu()
{
    //dtor
}

void CreditsMenu::addEntity( Entity * entity ){
}

void CreditsMenu::update(float dt){
}

void CreditsMenu::input( const sf::Event & event ){
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
}

void CreditsMenu::draw()
{
    Display::window->draw(arena);
    for ( std::vector<GuiButton*>::iterator it = m_buttons.begin(); it != m_buttons.end(); it++ ){
        (*it)->draw();
    }
    for ( std::vector<GuiLabel*>::iterator it = m_labels.begin(); it != m_labels.end(); it++ ){
        (*it)->draw();
    }
}
