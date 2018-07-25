#include "LuaConsole.h"
#include "Console.h"
#include "StageManager.h"
#include "SyncManager.h"
#include <string.h>

#include <iostream>

#include <SFML/Audio.hpp>

#include "GameRegistry.h"

#include "Config.h"

lua_State * LuaConsole::state = NULL;
bool LuaConsole::triggerUpdate = false;

#include "ObjectAPI.cpp"
#include "LabelAPI.cpp"
#include "EventAPI.cpp"

//LUA print() redefinition
int l_my_print(lua_State* L) {
    int nargs = lua_gettop(L);

    for (int i=1; i <= nargs; i++) {
        if (lua_isstring(L, i)) {
            /* Pop the next arg using lua_tostring(L, i) and do your print */
              puts( lua_tostring(L,i) );
              Console::pushTextElement( lua_tostring(L,i) );
        }
        else {
        /* Do something with non-strings if you like */
        }
    }

    return 0;
}

// GetWindowSize()
int l_GetWindowSize(lua_State* L) {
    lua_pushnumber(L, WINDOW_WIDTH );
    lua_pushnumber(L, WINDOW_HEIGHT );
    return 2;
}


void LuaConsole::triggerClientEvent( const char * eventName ){
    lua_pushstring(LuaConsole::state, eventName);
    lua_gettable(LuaConsole::state, LUA_REGISTRYINDEX);  /* retrieve value */

    //Now the function is on top of the stack
    lua_call(LuaConsole::state,0,0);
}

void LuaConsole::triggerKeyPressEvent( sf::Uint16 key ){
    if ( triggerUpdate ){
        lua_pushstring(LuaConsole::state, "onKeyPress");
        lua_gettable(LuaConsole::state, LUA_REGISTRYINDEX);  /* retrieve value */

        if ( lua_isnil(LuaConsole::state,-1) ) // if no function is registered for the event
            return;

        lua_pushnumber( LuaConsole::state, key );

        //Now the function is on top of the stack
        lua_call(LuaConsole::state,1,0); // call it - 1 arg, 0 ret
    }
}

void LuaConsole::triggerKeyReleaseEvent( sf::Uint16 key ){
    if ( triggerUpdate ){
        lua_pushstring(LuaConsole::state, "onKeyRelease");
        lua_gettable(LuaConsole::state, LUA_REGISTRYINDEX);  /* retrieve value */

        if ( lua_isnil(LuaConsole::state,-1) ) // if no function is registered for the event
            return;

        lua_pushnumber( LuaConsole::state, key );

        //Now the function is on top of the stack
        lua_call(LuaConsole::state,1,0); // call it - 1 arg, 0 ret
    }
}

void LuaConsole::triggerTypedEvent(char typedChar){
    if ( triggerUpdate ){
        lua_pushstring(LuaConsole::state, "onTyped");
        lua_gettable(LuaConsole::state, LUA_REGISTRYINDEX);  /* retrieve value */

        if ( lua_isnil(LuaConsole::state,-1) ) // if no function is registered for the event
            return;

        lua_pushlstring( LuaConsole::state, &typedChar, 1 );

        //Now the function is on top of the stack
        lua_call(LuaConsole::state,1,0); // call it - 1 arg, 0 ret
    }
}


void LuaConsole::triggerUpdateEvent(){
    if ( triggerUpdate ){
        lua_pushstring(LuaConsole::state, "onFrame");
        lua_gettable(LuaConsole::state, LUA_REGISTRYINDEX);

        if ( lua_isnil(LuaConsole::state,-1) ) // if no function is registered for the event
            return;

        //Now the function is on top of the stack
        lua_call(LuaConsole::state,0,0);
    }
}

void LuaConsole::triggerClientEvent( sf::Uint16 eventCode, sf::Packet restOfPacket ){
    //std::cout << "DEBUG 1 " << std::endl;

    std::string eventName = SyncManager::getClientEventName(eventCode);

    if ( eventName != "" ){ // does it work like that with std::string and cstr?

        //std::cout << "DEBUG 2 " << eventName << std::endl;

        lua_pushstring(LuaConsole::state, eventName.c_str()); // push the event name
        lua_gettable(LuaConsole::state, LUA_REGISTRYINDEX); // retrieve the function

        if ( ! lua_isnil(LuaConsole::state,-1) ){

            //std::cout << "DEBUG 3 " << std::endl;

            sf::Uint8 encoding;
            restOfPacket >> encoding;
            char index = 1;

            float floatBuffer;
            std::string stringBuffer;

            int args = 0;

            while ( !restOfPacket.endOfPacket() ){
                if ( index & encoding ){ // 1 for string, 0 for number
                    // string
                    restOfPacket >> stringBuffer;
                    lua_pushstring(LuaConsole::state,stringBuffer.c_str());
                    std::cout << "Pushed a string : " << stringBuffer.c_str() << std::endl;
                }else
                {
                    restOfPacket >> floatBuffer;
                    lua_pushnumber(LuaConsole::state, floatBuffer);
                    std::cout << "Pushed a float : " << floatBuffer << std::endl;
                }

                index = index << 1;
                args++;
            }

            //std::cout << "DEBUG 4 " << std::endl;

            std::cout << "args : " << args << std::endl;

            //Now the function is below the args
            lua_call(LuaConsole::state,args,0);
        }
        return;
    }

    std::cout << "No such event with code " << eventCode << std::endl;
}

// connect("127.0.0.1")
int l_ConnectToServer(lua_State* L) {

    char buffer[256];
    strcpy(buffer,lua_tostring(L,1));

    SyncManager::connectToServer(sf::IpAddress(buffer));

    return 0;
}

// GetNickname()
int l_GetNickname(lua_State * L){
    lua_pushstring(L,SyncManager::nickname.c_str());
    return 1;
}

sf::Sound sound;
int l_PlaySound(lua_State *L){

    char buffer[256];
    strcpy(buffer,lua_tostring(L,1));

    sound.setBuffer( * (GameRegistry::getResource(buffer,ResourceType::SoundBuffer).soundBuffer) );
    sound.play();

    return 0;
}


void LuaConsole::init(){
    if ( state == NULL ){
        state = luaL_newstate();

        // Make standard libraries available in the Lua object
        luaL_openlibs(state);
        luaopen_math();

        lua_pushcfunction(state, l_my_print);
        lua_setglobal(state, "print");

        lua_pushcfunction(state, l_SetPosition);
        lua_setglobal(state, "SetPosition");

        lua_pushcfunction(state, l_GetPosition);
        lua_setglobal(state, "GetPosition");

        //lua_pushcfunction(state, l_RegisterServerEvent);
        //lua_setglobal(state, "RegisterServerEvent");

        lua_pushcfunction(state, l_RegisterClientEvent);
        lua_setglobal(state, "RegisterClientEvent");

        lua_pushcfunction(state, l_TriggerServerEvent);
        lua_setglobal(state, "TriggerServerEvent");

        lua_pushcfunction(state, l_AddEventHandler);
        lua_setglobal(state, "AddEventHandler");

        lua_pushcfunction(state, l_CreateObject);
        lua_setglobal(state, "CreateObject");

        lua_pushcfunction(state, l_SetFriction);
        lua_setglobal(state, "SetFriction");

        lua_pushcfunction(state, l_SetRotation);
        lua_setglobal(state, "SetRotation");

        lua_pushcfunction(state, l_SetVelocity);
        lua_setglobal(state, "SetVelocity");

        lua_pushcfunction(state, l_SetTexture);
        lua_setglobal(state, "SetTexture");

        lua_pushcfunction(state, l_KillObject);
        lua_setglobal(state, "KillObject");

        lua_pushcfunction(state, l_RegisterAsServerObject);
        lua_setglobal(state, "RegisterAsServerObject");

        lua_pushcfunction(state, l_TriggerServerEvent);
        lua_setglobal(state, "TriggerServerEvent");

        lua_pushcfunction(state, l_GetController);
        lua_setglobal(state, "GetController");



        lua_pushcfunction(state, l_CreateLabel);
        lua_setglobal(state, "CreateLabel");

        lua_pushcfunction(state, l_SetLabelPosition);
        lua_setglobal(state, "SetLabelPosition");

        lua_pushcfunction(state, l_GetBounds);
        lua_setglobal(state, "GetBounds");

        lua_pushcfunction(state, l_SetText);
        lua_setglobal(state, "SetText");

        lua_pushcfunction(state, l_SetFontSize);
        lua_setglobal(state, "SetFontSize");

        lua_pushcfunction(state, l_SetColor);
        lua_setglobal(state, "SetColor");



        lua_pushcfunction(state, l_DestroyLabel);
        lua_setglobal(state, "DestroyLabel");

        lua_pushcfunction(state, l_GetWindowSize);
        lua_setglobal(state, "GetWindowSize");

        lua_pushcfunction(state, l_ConnectToServer);
        lua_setglobal(state, "connect");


        lua_pushcfunction(state, l_GetNickname);
        lua_setglobal(state, "GetNickname");

        lua_pushcfunction(state, l_PlaySound);
        lua_setglobal(state, "PlaySound");
    }
}

void LuaConsole::print_error(lua_State* state) {
  // The error message is on top of the stack.
  // Fetch it, print it and then pop it off the stack.
  const char* message = lua_tostring(state, -1);
  puts(message);
  Console::pushTextElement(message);
  lua_pop(state, 1);
}

void LuaConsole::execute(const char* filename)
{
  int result;

  // Load the program; this supports both source code and bytecode files.
  result = luaL_loadfile(state, filename);

  if ( result != LUA_OK ) {
    print_error(state);
    return;
  }

  // Finally, execute the program by calling into it.
  // Change the arguments if you're not running vanilla Lua code.

  printf("[Lua] File loaded correctly %s\n", filename);

  result = lua_pcall(state, 0, LUA_MULTRET, 0);

  if ( result != LUA_OK ) {
    print_error(state);
    return;
  }
}

void LuaConsole::executeLine( const char * s ){
    int result = luaL_loadstring ( state, s );
    switch( result ){
        case LUA_ERRSYNTAX:
            puts("[Lua] Error executing line ( syntax ) !");
            Console::pushTextElement("[Lua] Error executing line ( syntax ) !");
            break;
        case LUA_ERRMEM:
            puts("[Lua] Error executing line ( memory ) !");
            Console::pushTextElement("[Lua] Error executing line ( memory ) !");
            break;
        default:{
            int res = lua_pcall(state,0,LUA_MULTRET, 0 );
            if ( res != LUA_OK ) {
                print_error(state);
                return;
            }
        }
    }
}
