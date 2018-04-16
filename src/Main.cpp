#include <SFML/Graphics.hpp>

#include "Display.h"
#include "GameRegistry.h"
#include "StageManager.h"

#include "SyncManager.h"
#include "Config.h"

#include "Emeraldo.h"
#include "Rubie.h"
#include "Sapheer.h"
#include "Particle.h"

#include <time.h>
#include <stdlib.h>

#include "DebugLog.h"

#include "LuaConsole.h"

int main(){
    srand(time(NULL));

    GameRegistry::init();
    Display::init();
    StageManager::init();

    SyncManager::init();

    sf::View view;

    view.setSize( WINDOW_WIDTH, WINDOW_HEIGHT );
/*
    ///Debugging snippet///
    SyncManager::connectToServer("127.0.0.1");
    SyncManager::sendCrystalType(2); // Emeraldo
    StageManager::pushStage(StageManager::gameState);
    ///End of debugging snippet///
*/
    sf::CircleShape circle;
    circle.setRadius(50.f);
    circle.setFillColor(sf::Color::Green);

    LuaConsole::init();
    LuaConsole::execute("resources/crystal/client.lua");

    SyncManager::connectToServer(sf::IpAddress("127.0.0.1"));

    while (Display::window->isOpen()){

        StageManager::updateCurrentStage();

        sf::Event event;
        while (Display::window->pollEvent(event)){

            if (event.type == sf::Event::Closed)
                Display::window->close();
            if (event.type == sf::Event::KeyPressed ){
                if ( event.key.code == sf::Keyboard::Escape )
                    Display::window->close();
                LuaConsole::triggerKeyPressEvent( event.key.code );
            }
            StageManager::input(event);
        }

        SyncManager::receivePackets();

        StageManager::update(10.f);

        Display::window->clear();
        StageManager::draw();
        Display::window->display();
    }

    return 0;
}
