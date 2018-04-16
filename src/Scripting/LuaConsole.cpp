#include "LuaConsole.h"
#include "Console.h"
#include "StageManager.h"
#include "SyncManager.h"
#include <string.h>


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

    int objectID;
    float x,y;

    if ( luaL_checkinteger(L,-3) ){
        objectID = lua_tonumber(L,-3);
    }

    x = lua_tonumber(L,-2);
    y = lua_tonumber(L,-1);

    StageManager::gameState->getObject(objectID)->setPosition(x,y);

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
    strcpy(eventName,lua_tostring(L,-1));
    SyncManager::triggerServerEvent(eventName);

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


void LuaConsole::init(){
    if ( state == NULL ){
        state = luaL_newstate();

        // Make standard libraries available in the Lua object
        luaL_openlibs(state);

        lua_pushcfunction(state, l_my_print);
        lua_setglobal(state, "print");
        lua_pushcfunction(state, l_SetPosition);
        lua_setglobal(state, "SetPosition");

        lua_pushcfunction(state, l_RegisterServerEvent);
        lua_setglobal(state, "RegisterServerEvent");

        lua_pushcfunction(state, l_TriggerServerEvent);
        lua_setglobal(state, "TriggerServerEvent");

        lua_pushcfunction(state, l_AddEventHandler);
        lua_setglobal(state, "AddEventHandler");

        lua_pushcfunction(state, l_CreateObject);
        lua_setglobal(state, "CreateObject");


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
