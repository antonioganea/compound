#include "GameState.h"

#include <iostream>

//#include "SyncManager.h"
#include "GameRegistry.h"
#include "Display.h"

#include "Object.h"

GameState::GameState()
{
    //allocate object containers

    std::cout << "GameState created!\n";

    sf::Texture * arenaTexture = GameRegistry::getResource("icepattern.png",ResourceType::Texture).texture;
    arena.setSize((sf::Vector2f)arenaTexture->getSize());
    arena.setTexture(arenaTexture);
    arena.setOrigin((sf::Vector2f)arenaTexture->getSize()/2.f);
    arena.setScale(16.f,16.f);

    circle.setRadius(5000);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Black);
    circle.setPointCount(100);
    circle.setOutlineThickness(5);
    circle.setOrigin(2500,2500);
    circle.setPosition(0,0);
}

GameState::~GameState()
{
    //dtor
}

Object * GameState::getObject( sf::Uint16 objectID ){
    return m_objects[objectID];
}

Object * GameState::getObjectByServerID( sf::Uint16 serverID ){
    return m_objectsByServerID[serverID];
}

sf::Uint16 GameState::registerClientObject( Object * object ){
    for ( sf::Uint16 i = 0; i < maxClientObject; i++ ){
        if ( m_objects[i] == 0 ){
            m_objects[i] = object;
            object->setClientID(i);
            std::cout << "Added client object to gamestate! id : " << i << std::endl;
            return i;
        }
    }
    m_objects[maxClientObject] = object;
    object->setClientID(maxClientObject);
    std::cout << "Added client object to gamestate! id : " << maxClientObject << std::endl;
    maxClientObject++;
    return maxClientObject-1;
    // what if this overflows?
}

void GameState::addEntity(Entity* entity){
    return;
}


void GameState::update(float dt){
    for ( sf::Uint16 i = 0; i < MAX_OBJECTS; i++ ){
        if ( m_objects[i] != 0 ){
            m_objects[i]->update(dt);
            if ( m_objects[i]->isDead() ){
                delete m_objects[i];
                m_objects[i] = 0;
                std::cout << "Deleted object!" << std::endl;
            }
        }
    }
}

void GameState::input( const sf::Event & event )
{
    //SyncManager::input(event);
    //emeraldo->input( event );
}

void GameState::draw()
{
    Display::window->draw(arena);
    Display::window->draw(circle);

    for ( sf::Uint16 i = 0; i < MAX_OBJECTS; i++ ){
        if ( m_objects[i] != 0 ){
            m_objects[i]->draw();
        }
    }
}


void GameState::bindServerIDtoClientObject(sf::Uint16 serverID, sf::Uint16 clientID){
    m_objectsByServerID[serverID] = m_objects[clientID];
}
