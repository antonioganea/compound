#include "GameState.h"

#include <iostream>

//#include "SyncManager.h"
#include "GameRegistry.h"
#include "Display.h"

//#include "Emeraldo.h"
//Emeraldo * emeraldo;

#include "Object.h"

Object * myObject;

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

    //testCrystal = new Emeraldo();


    //emeraldo = new Emeraldo();
    //addEntity( emeraldo );

    myObject = new Object();
    addEntity(myObject);

}

GameState::~GameState()
{
    //dtor
}

Object * GameState::getObject( int objectID ){
    return myObject;
}

void GameState::addEntity( Entity * entity )
{
    std::cout << "Added object to gamestate!" << std::endl;
    ps_entities.push_back( entity );
}

void GameState::update(float dt)
{
    for ( std::vector<Entity*>::iterator it = m_entities.begin(); it != m_entities.end();)
    {
        (*it)->update(dt);
        if ( (*it)->isDead() ){
            std::cout<<"ERASED ENTITY"<<std::endl;
            delete (*it);
            it = m_entities.erase(it);
        }
        else
            ++it;
    }

    while ( ps_entities.size() )
    {
        m_entities.push_back ( ps_entities.back() );
        ps_entities.pop_back();
    }
}

void GameState::input( const sf::Event & event )
{
    //SyncManager::input(event);

    //emeraldo->input( event );
}

void GameState::draw()
{
    // These values might be NULLs - implement NULL guards

    /*
    if ( SyncManager::myPlayerID != -1 ){ // need better function for this checking
        if ( SyncManager::crystals[SyncManager::myPlayerID] != NULL ){ // need better function for this checking
            Display::focusOn(SyncManager::crystals[SyncManager::myPlayerID]);
        }
    }
    */

    //Crystal * viewTarget = 0;
    /*
    if ( SyncManager::myPlayerID != -1 ){ // need better function for this checking
        if ( SyncManager::crystals[SyncManager::myPlayerID] != NULL ){ // need better function for this checking
            //Display::focusOn(SyncManager::crystals[SyncManager::myPlayerID]);
            viewTarget = SyncManager::crystals[SyncManager::myPlayerID];
        }
    }
    if ( !viewTarget ){
        for ( int i = 0; i < MAX_PLAYERS; i++ ){
            if ( SyncManager::crystals[i] ){
                viewTarget = SyncManager::crystals[i];
                break;
            }
        }
    }*/

    //Display::focusOn(viewTarget);

    //Display::focusOn(myObject);

    Display::window->draw(arena);
    Display::window->draw(circle);
    for ( std::vector<Entity*>::iterator it = m_entities.begin(); it != m_entities.end(); ++it )
    {
        (*it)->draw();
    }
}
