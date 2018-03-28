#ifndef STAGE_H
#define STAGE_H

#include "Entity.h"

class Stage
{
    public:
        virtual void addEntity ( Entity * entity ) = 0;
        virtual void update( float dt ) = 0;
        virtual void input( const sf::Event & event ) = 0;
        virtual void draw() = 0;
    protected:

    private:

};

#endif // STAGE_H
