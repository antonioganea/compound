#include "Console.h"

#include "Display.h"


#include "LuaConsole.h"

#include "SyncManager.h"

float triangleSize = 10.f;

sf::Font myfont;

char currentText[256];
int index = 0;

float Console::coverage = 0.3f;
bool Console::visible = false;

sf::RectangleShape Console::background;
sf::CircleShape Console::triangle;
sf::Text Console::currentInput;
sf::Text Console::label;
float Console::width, Console::height;

#define TEXT_ELEMENTS 18

sf::Text * elements[TEXT_ELEMENTS];

void Console::generateTextElements(){
    for ( int i = 0; i < TEXT_ELEMENTS; i++ ){
        sf::Text * newText = new sf::Text();
        newText->setString("");
        newText->setFillColor(sf::Color::Green);
        newText->setCharacterSize(16);
        newText->setFont(myfont);
        newText->setPosition(5,i*16);
        elements[i] = newText;
    }
}

void Console::pushTextElement ( const char * buffer ){
    for ( int i = 0; i < TEXT_ELEMENTS-1; i++ ){
        elements[i]->setString( elements[i+1]->getString() );
    }
    elements[TEXT_ELEMENTS-1]->setString(buffer);
}

void Console::init()
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

    currentInput.setFillColor(sf::Color::Green);
    //currentInput.setString("HELLOWORLD");
    currentInput.setCharacterSize(16);
    currentInput.setFont(myfont);
    currentInput.setPosition(triangleSize*2,height-triangleSize*2);

    label.setColor(sf::Color::Green);
    label.setString("Compound Console v0.3");
    label.setCharacterSize(16);
    label.setFont(myfont);
    label.setPosition(width-label.getLocalBounds().width-3,height-label.getLocalBounds().height);

    setVisible(true);

    generateTextElements();
}

void Console::setVisible(bool _visible)
{
    visible = _visible;
}

bool Console::getVisible()
{
    return visible;
}


bool Console::input(sf::Event event)
{
    if ( event.type == sf::Event::KeyPressed ){
        if ( event.key.code == sf::Keyboard::Tilde ){
            visible = !visible;
            return true;
        }
        if ( visible )
            return true;
    }


    if ( event.type == sf::Event::TextEntered ){
        if ( visible && event.text.unicode != '`' ){
            if ( ( !isspace(event.text.unicode) || event.text.unicode == ' ' ) && event.text.unicode != 8 ){
                currentText[index] = event.text.unicode;
                currentText[index+1] = 0;
                index++;
                currentInput.setString(currentText);
                //printf("INDEX++ .. %d %c\n",index,event.text.unicode);
                return true;
            }else if ( event.text.unicode == 8 ){ // backspace
                index--;
                if ( index < 0 )
                    index = 0;
                currentText[index] = 0;
                currentInput.setString(currentText);
                return true;
            }else if ( event.text.unicode == 13 ){ // Carriage return / enter
                pushTextElement(currentText);
                LuaConsole::executeLine(currentText);
                //SyncManager::sendTCPMessage(currentText,index);
                //SyncManager::sendUDPMessage(currentText,index);
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
        Display::window->setView(Display::GuiView);
        Display::window->draw(background);
        Display::window->draw(triangle);
        Display::window->draw(currentInput);
        Display::window->draw(label);

        for ( int i = 0; i < TEXT_ELEMENTS; i++ ){
            //elements[i]->setPosition(0,i*16);
            Display::window->draw( *elements[i] );
        }
        Display::window->setView(Display::view);
    }
}
