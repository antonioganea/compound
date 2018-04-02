#include "Console.h"

#include <Display.h>

float triangleSize = 10.f;

sf::Font myfont;

char currentText[256];
int index = 0;

sf::Text * elements[10];
int currentElement = 0;

void Console::pushTextElement(char* buffer){
    if ( currentElement < 10 ){
        sf::Text * newText = new sf::Text();
        newText->setString(buffer);
        newText->setColor(sf::Color::Green);
        newText->setCharacterSize(16);
        newText->setFont(myfont);
        newText->setPosition(0,0);
        elements[currentElement] = newText;
        currentElement++;
    }
}

Console::Console()
{
    background.setFillColor(sf::Color(113, 214, 98,64));
    sf::Vector2u consoleSize = Display::window->getSize();
    consoleSize.y = (consoleSize.y * coverage);
    background.setSize( (sf::Vector2f)consoleSize );
    background.setOutlineColor(sf::Color::Green);
    background.setOutlineThickness(1.f);
    width = consoleSize.x;
    height = consoleSize.y;

    triangle.setRadius(triangleSize);
    triangle.setFillColor(sf::Color::Green);
    triangle.setPointCount(3);
    triangle.setRotation(90);
    triangle.setOrigin(triangleSize/2.f,triangleSize/2.f);
    triangle.setPosition(triangleSize*1.5,height-triangleSize*1.5);

    myfont.loadFromFile("data/fonts/Ubuntu-R.ttf");

    currentInput.setColor(sf::Color::Green);
    //currentInput.setString("HELLOWORLD");
    currentInput.setCharacterSize(16);
    currentInput.setFont(myfont);
    currentInput.setPosition(triangleSize*2,height-triangleSize*2);

    label.setColor(sf::Color::Green);
    label.setString("Compound Console v0.1");
    label.setCharacterSize(16);
    label.setFont(myfont);
    label.setPosition(width-label.getLocalBounds().width-3,height-label.getLocalBounds().height);

    setVisible(true);
}

Console::~Console()
{
    //dtor
}

void Console::setVisible(bool _visible)
{
    visible = _visible;
}

bool Console::input(sf::Event event)
{
    if ( event.type == sf::Event::KeyPressed ){
        if ( event.key.code == sf::Keyboard::Tilde ){
            visible = !visible;
            return true;
        }
    }


    if ( event.type == sf::Event::TextEntered ){
        if ( visible && event.text.unicode != '`' ){
            if ( ( !isspace(event.text.unicode) || event.text.unicode == ' ' ) && event.text.unicode != 8 ){
                currentText[index] = event.text.unicode;
                currentText[index+1] = 0;
                index++;
                currentInput.setString(currentText);
                //printf("INDEX++ .. %d %c\n",index,event.text.unicode);
            }else if ( event.text.unicode == 8 ){ // backspace
                index--;
                if ( index < 0 )
                    index = 0;
                currentText[index] = 0;
                currentInput.setString(currentText);
            }else if ( event.text.unicode == 13 ){ // Carriage return / enter
                pushTextElement(currentText);
                index = 0;
                currentText[0] = 0;
                currentInput.setString(currentText);
            }
            return true;
        }
    }
    return false;
}

void Console::draw()
{

    if ( visible ){
        Display::window->draw(background);
        Display::window->draw(triangle);
        Display::window->draw(currentInput);
        Display::window->draw(label);

        int start = 0;
        for ( int i = 0; i < currentElement; i++ ){
            elements[i]->setPosition(0,i*16);
            Display::window->draw( *elements[i] );
        }
    }
}
