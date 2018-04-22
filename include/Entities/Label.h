#ifndef LABEL_H
#define LABEL_H

#include "Entity.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class Label : public Entity
{
    public:
        Label();
        virtual ~Label();

        void draw();
        void update( float dt );
        bool isDead();

        bool dead;
        sf::Font * font;
        sf::Text text;
        void setFont( sf::Font f );
        void setPosition( int x, int y );
        void setSize ( int size );
        void setColor ( int id );
        void destroy();
        void setText( char * str );

    protected:

    private:
};

#endif // LABEL_H
