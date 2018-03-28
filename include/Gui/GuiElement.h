#ifndef GUIELEMENT_H_INCLUDED
#define GUIELEMENT_H_INCLUDED

class GuiElement : public Entity
{
    public:
        virtual void draw() = 0;
        virtual void update( float dt ) = 0;
        virtual bool isDead() = 0;

        virtual void input( const sf::Event & event ) = 0;

        virtual void setPosition ( const sf::Vector2f& position ) = 0;
        virtual sf::Vector2f getPosition() = 0;

        virtual bool isCollidable() = 0;
    protected:
    private:
};

#endif // GUIELEMENT_H_INCLUDED
