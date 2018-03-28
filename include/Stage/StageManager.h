#ifndef STAGEMANAGER_H
#define STAGEMANAGER_H

#include "Stage.h"
#include "GameState.h"
#include "MainMenu.h"
#include "CreditsMenu.h"
#include "ConnectMenu.h"
#include "PickMenu.h"

#include <stack>

class StageManager
{
    public:
        static void init();
        static void pushStage( Stage * stage );
        static void popStage();
        static Stage* getStage();
        static void update( float dt );
        static void input( const sf::Event & event );
        static void draw();
        static void updateCurrentStage();

        static GameState * gameState;
        static MainMenu * mainMenu;
        static CreditsMenu * creditsMenu;
        static ConnectMenu * connectMenu;
        static PickMenu * pickMenu;
    protected:

    private:
        StageManager();
        static Stage * currentStage;
        static std::stack<Stage*> m_stages;
};

#endif // STAGEMANAGER_H
