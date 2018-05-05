#include "StageManager.h"

#include "GameState.h"
#include "Display.h"

#include <iostream>

#include <Console.h>

Stage * StageManager::currentStage;

GameState * StageManager::gameState;
MainMenu * StageManager::mainMenu;
CreditsMenu * StageManager::creditsMenu;
ConnectMenu * StageManager::connectMenu;
PickMenu * StageManager::pickMenu;

std::stack<Stage*> StageManager::m_stages;

void StageManager::init()
{
    StageManager::gameState = new GameState;
    StageManager::mainMenu = new MainMenu;
    StageManager::creditsMenu = new CreditsMenu;
    StageManager::connectMenu = new ConnectMenu;
    StageManager::pickMenu = new PickMenu;

    StageManager::currentStage = mainMenu;
    m_stages.push(currentStage);

    //Debug :
    //pushStage(gameState);
    pushStage(mainMenu);

    Console::init();
}

void StageManager::pushStage( Stage * stage ){
    if ( stage != m_stages.top() ){
        m_stages.push(stage);
        Display::resetView();
    }
}

void StageManager::popStage(){
    if ( m_stages.size() > 1 ){
        m_stages.pop();
        Display::resetView();
    }
}

Stage* StageManager::getStage()
{
    return StageManager::currentStage;
}

void StageManager::update(float dt)
{
    StageManager::currentStage->update(dt);
}

/** This is the first "StageManager" function to run in the loop */
void StageManager::updateCurrentStage(){
    currentStage = m_stages.top();
}

void StageManager::input( const sf::Event & event )
{
    if ( Console::input( event ) )
        return;
    currentStage->input( event );
}

void StageManager::draw()
{
    currentStage->draw();
    Console::draw();
}

StageManager::StageManager()
{

}
