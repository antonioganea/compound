#include "LuaConsole.h"
#include "Console.h"
#include "StageManager.h"
#include "SyncManager.h"
#include <string.h>

#include <iostream>


lua_State * LuaConsole::state = NULL;


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

// SetPosition( object, x, y )
int l_SetPosition(lua_State* L) {
    sf::Uint16 objectID;
    float x,y;

    if ( luaL_checkinteger(L,-3) ){
        objectID = lua_tonumber(L,-3);
    }

    x = lua_tonumber(L,-2);
    y = lua_tonumber(L,-1);

    Object * obj = StageManager::gameState->getObject(objectID);
    //std::cout << "getObject : " << obj << std::endl;
    if ( obj != 0 ){
        sf::Packet newPacket = obj->setPosition(x,y);
        if ( obj->getSynced() )
            SyncManager::sendTCPMessage( newPacket );
    }


    return 0;
}

// SetVelocity( object, x, y )
int l_SetVelocity(lua_State* L) {

    sf::Uint16 objectID;
    float x,y;

    if ( luaL_checkinteger(L,-3) ){
        objectID = lua_tonumber(L,-3);
    }

    x = lua_tonumber(L,-2);
    y = lua_tonumber(L,-1);

    Object * obj = StageManager::gameState->getObject(objectID);
    if ( obj != 0 ){
        sf::Packet newPacket = obj->setVelocity(x,y);
        if ( obj->getSynced() )
            SyncManager::sendTCPMessage( newPacket );
    }


    return 0;
}

// SetRotation( object, rot )
int l_SetRotation(lua_State* L) {

    sf::Uint16 objectID;
    float rot;

    if ( luaL_checkinteger(L,-2) ){
        objectID = lua_tonumber(L,-2);
    }

    rot = lua_tonumber(L,-1);

    Object * obj = StageManager::gameState->getObject(objectID);
    if ( obj != 0 ){
        sf::Packet newPacket = obj->setRotation(rot);
        if ( obj->getSynced() )
            SyncManager::sendTCPMessage( newPacket );
    }


    return 0;
}

// SetFriction( object, friction )
int l_SetFriction(lua_State* L) {

    sf::Uint16 objectID;
    float friction;

    if ( luaL_checkinteger(L,-2) ){
        objectID = lua_tonumber(L,-2);
    }

    friction = lua_tonumber(L,-1);

    Object * obj = StageManager::gameState->getObject(objectID);
    if ( obj != 0 ){
        sf::Packet newPacket = obj->setFriction(friction);
        if ( obj->getSynced() )
            SyncManager::sendTCPMessage( newPacket );
    }

    return 0;
}

// SetTexture( object, textureID )
int l_SetTexture(lua_State* L) {

    sf::Uint16 objectID;
    sf::Uint16 textureID;

    if ( luaL_checkinteger(L,-2) ){
        objectID = lua_tonumber(L,-2);
    }

    textureID = lua_tonumber(L,-1);

    Object * obj = StageManager::gameState->getObject(objectID);
    if ( obj != 0 ){
        sf::Packet newPacket = obj->setTextureID(textureID);
        if ( obj->getSynced() )
            SyncManager::sendTCPMessage( newPacket );
    }

    return 0;
}

// KillObject( object )
int l_KillObject(lua_State* L) {

    sf::Uint16 objectID;

    if ( luaL_checkinteger(L,-1) ){
        objectID = lua_tonumber(L,-1);
    }

    Object * obj = StageManager::gameState->getObject(objectID);
    if ( obj != 0 ){
        sf::Packet newPacket = obj->kill();
        if ( obj->getSynced() )
            SyncManager::sendTCPMessage( newPacket );
    }

    return 0;
}

// RegisterAsServerObject( object )
int l_RegisterAsServerObject(lua_State* L) {
    std::cout << "Register as server object" << std::endl;
    sf::Uint16 objectID;

    if ( luaL_checkinteger(L,-1) ){
        objectID = lua_tonumber(L,-1);
    }

    std::cout << objectID << std::endl;

    Object * obj = StageManager::gameState->getObject(objectID);
    //std::cout << obj << std::endl;
    if ( obj != 0 ){
        //std::cout << "REGISTERING" << std::endl;
        SyncManager::registerObjectToServer( obj );
    }

    return 0;
}

// CreateObject()
int l_CreateObject(lua_State* L){
    Object * newObject = new Object();
    sf::Uint16 id = StageManager::gameState->registerClientObject( newObject );
    lua_pushnumber( L, id );

    return 1;
}

// RegisterServerEvent ( eventName, eventCode )
// This is left for debug
int l_RegisterServerEvent(lua_State* L) {

    char eventName[256];

    strcpy(eventName,lua_tostring(L,-2));
    int eventCode = lua_tonumber(L,-1);

    SyncManager::registerServerEvent(eventName,eventCode);

    return 0;
}

// TriggerServerEvent ( eventName )
int l_TriggerServerEvent(lua_State* L) {

    char eventName[256];
    strcpy(eventName,lua_tostring(L,1));

    int args = lua_gettop(L);

    if ( args == 1){
        SyncManager::triggerServerEvent(eventName);
    }else{
        sf::Packet newPacket;

        float floatBuffer;
        char stringBuffer[256];

        sf::Uint16 eventCode = SyncManager::getServerEventCode(eventName);
        newPacket << eventCode;
        sf::Uint8 encoding = 0;
        sf::Uint8 index = 1;
        for ( int i = 2; i <= args; i++ ){
            if ( !lua_isnumber(L,i) ){//0 for numbers, 1 for strings
                encoding = encoding | index;
            }
            index = index << 1;
        }

        newPacket << encoding;
        for ( int i = 2; i <= args; i++ ){
            if ( lua_isnumber(L,i) ){
                floatBuffer = lua_tonumber(L,i);
                newPacket << floatBuffer;
            }else{
                strcpy(stringBuffer,lua_tostring(L,i));
                newPacket << stringBuffer;
            }
        }

        SyncManager::sendTCPMessage(newPacket);

    }

    return 0;
}

// RegisterClientEvent ( eventName )
int l_RegisterClientEvent(lua_State* L) {
    char eventName[256];

    strcpy(eventName,lua_tostring(L,-1));

    SyncManager::requestClientEvent(eventName);

    return 0;
}

// AddEventHandler ( eventName, eventHandler )
int l_AddEventHandler(lua_State* L) {

    //char eventName[256];
    //strcpy(eventName,lua_tostring(L,-2));

    //TODO : checktypes

    lua_settable(L, LUA_REGISTRYINDEX);

    return 0;
}

void LuaConsole::triggerClientEvent( const char * eventName ){
    lua_pushstring(LuaConsole::state, eventName);
    lua_gettable(LuaConsole::state, LUA_REGISTRYINDEX);  /* retrieve value */

    //Now the function is on top of the stack
    lua_call(LuaConsole::state,0,0);
}

void LuaConsole::triggerKeyPressEvent( sf::Uint16 key ){
    lua_pushstring(LuaConsole::state, "onKeyPress");
    lua_gettable(LuaConsole::state, LUA_REGISTRYINDEX);  /* retrieve value */

    if ( lua_isnil(LuaConsole::state,-1) ) // if no function is registered for the event
        return;

    lua_pushnumber( LuaConsole::state, key );

    //Now the function is on top of the stack
    lua_call(LuaConsole::state,1,0); // call it - 1 arg, 0 ret
}

void LuaConsole::triggerUpdateEvent(){
    lua_pushstring(LuaConsole::state, "onFrame");
    lua_gettable(LuaConsole::state, LUA_REGISTRYINDEX);

    if ( lua_isnil(LuaConsole::state,-1) ) // if no function is registered for the event
        return;

    //Now the function is on top of the stack
    lua_call(LuaConsole::state,0,0);
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


void LuaConsole::init(){
    if ( state == NULL ){
        state = luaL_newstate();

        // Make standard libraries available in the Lua object
        luaL_openlibs(state);

        lua_pushcfunction(state, l_my_print);
        lua_setglobal(state, "print");
        lua_pushcfunction(state, l_SetPosition);
        lua_setglobal(state, "SetPosition");

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
