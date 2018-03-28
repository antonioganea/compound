#include "MainMenu.h"

#include <iostream>

#include "GameRegistry.h"
#include "Display.h"

#include "GuiButton.h"
#include "Config.h"

MainMenu::MainMenu()
{
    std::cout << "Main Menu created!\n";

    sf::Texture * arenaTexture = GameRegistry::getResource("arena.png",ResourceType::Texture).texture;
    arena.setSize( sf::Vector2f(WINDOW_WIDTH,WINDOW_HEIGHT) );
    arena.setTexture(arenaTexture);

    for ( int i = 0; i < 4; i++ ){
        GuiButton * button = new GuiButton(i+1);
        button->setPosition(WINDOW_WIDTH/2,500+120*i);
        m_buttons.push_back(button);
    }
}

MainMenu::~MainMenu()
{
    //dtor
}

void MainMenu::addEntity( Entity * entity ){
}

void MainMenu::update(float dt){
}

void MainMenu::input( const sf::Event & event ){
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

void MainMenu::draw()
{
    Display::window->draw(arena);
    for ( std::vector<GuiButton*>::iterator it = m_buttons.begin(); it != m_buttons.end(); it++ ){
        (*it)->draw();
    }
}
