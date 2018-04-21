#ifndef GAMESTATE_H
#define GAMESTATE_H

//#include <vector>

#include "Stage.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "Object.h"

// Maximum number of server-side ( sync'd objects )
#define MAX_SERVER_OBJECTS 1024

// Maximum number of clientside objects ( non-sync'd )
#define MAX_OBJECTS 1024

class GameState : public Stage
{
    public:
        GameState();
        virtual ~GameState();

        sf::Uint16 registerClientObject(Object * object);
        void addEntity ( Entity * entity );
        void update( float dt );
        void input( const sf::Event & event );
        void draw();
        sf::CircleShape circle;
        sf::RectangleShape arena;

        Object * getObject( sf::Uint16 objectID );
        Object * getObjectByServerID( sf::Uint16 serverID );

        void bindServerIDtoClientObject( sf::Uint16 serverID, sf::Uint16 clientID );

    protected:

    private:
        Object * m_objects[MAX_OBJECTS];
        Object * m_objectsByServerID[MAX_SERVER_OBJECTS];

        int maxClientObject = 0;
        int maxServerObject = 0;
};

#endif // GAMESTATE_H
