#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <SFML/Window/Event.hpp>

class Entity
{
    public:
        virtual void draw() = 0;
        virtual void update( float dt ) = 0;
        virtual bool isDead() = 0;
    protected:

    private:
};

#endif // ENTITY_H_INCLUDED
