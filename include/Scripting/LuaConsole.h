#ifndef LUACONSOLE_H
#define LUACONSOLE_H

#ifdef __cplusplus
# include <lua.hpp>
#else
# include <lua.h>
# include <lualib.h>
# include <lauxlib.h>
#endif

#include <SFML/System.hpp>

class LuaConsole
{
    public:
        static void init();
        static void print_error(lua_State* state);
        static void execute(const char* filename);
        static void executeLine( const char * s );

        static void triggerClientEvent( const char * eventName );
        static void triggerKeyPressEvent( sf::Uint16 key );
    protected:

    private:
        LuaConsole(); // static singleton
        static lua_State * state;
};

#endif // LUACONSOLE_H
