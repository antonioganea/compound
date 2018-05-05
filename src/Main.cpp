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
#include "Console.h"

int main(){
    srand(time(NULL));

    GameRegistry::init();
    Display::init();
    StageManager::init();

    SyncManager::init();

    sf::View view;

    view.setSize( WINDOW_WIDTH, WINDOW_HEIGHT );

    sf::CircleShape circle;
    circle.setRadius(50.f);
    circle.setFillColor(sf::Color::Green);

    LuaConsole::init();
    LuaConsole::executeLine("print('LUA LOADED')");

    //SyncManager::connectToServer(sf::IpAddress("127.0.0.1"));

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
            if (event.type == sf::Event::KeyReleased){
                LuaConsole::triggerKeyReleaseEvent( event.key.code );
            }

            if (event.type == sf::Event::TextEntered ){
                if ( !Console::getVisible() ){
                    if( ( 'a' <= event.text.unicode && 'z' >= event.text.unicode ) ||
                        ( 'A' <= event.text.unicode && 'Z' >= event.text.unicode ) ||
                        ( '0' <= event.text.unicode && '9' >= event.text.unicode )
                    )
                    LuaConsole::triggerTypedEvent(event.text.unicode);
                }
            }
            StageManager::input(event);
        }

        SyncManager::receivePackets();

        StageManager::update(10.f);
        LuaConsole::triggerUpdateEvent();

        Display::window->clear();
        StageManager::draw();
        Display::window->display();
    }

    return 0;
}
