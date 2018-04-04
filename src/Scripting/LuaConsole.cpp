#include "LuaConsole.h"
#include "Console.h"

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


void LuaConsole::init(){
    if ( state == NULL ){
        state = luaL_newstate();

        // Make standard libraries available in the Lua object
        luaL_openlibs(state);

        lua_pushcfunction(state, l_my_print);
        lua_setglobal(state, "print");
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
