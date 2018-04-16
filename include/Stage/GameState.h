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
#define MAX_CLIENT_OBJECTS 1024

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

    protected:

    private:
        //Object * m_serverObjects[MAX_SERVER_OBJECTS];
        //Object * m_clientObjects[MAX_CLIENT_OBJECTS];

        // One contiguous pointer to entity vector ensures that each entity has a unique id
        // So there isn't any synced object with the same id as a non-synced object
        // Server objects go first, client objects go second, because the server just stores the first part
        Object * m_objects[MAX_SERVER_OBJECTS+MAX_CLIENT_OBJECTS];

        sf::Uint16 maxServerObject = 0;
        sf::Uint16 maxClientObject = MAX_SERVER_OBJECTS;
};

#endif // GAMESTATE_H
