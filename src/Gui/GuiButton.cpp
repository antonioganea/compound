#include "GuiButton.h"

#include "Display.h"
#include "GameRegistry.h"
#include <iostream>

#include "StageManager.h"
#include "SyncManager.h"


GuiButton::GuiButton(int _functionality){
    functionality = _functionality;
    sf::Texture * buttonTex;
    switch( functionality ){
        case 1: // Play
            buttonTex = GameRegistry::getResource("playbtn-sheet.png",ResourceType::Texture).texture;
            break;
        case 2: // Options
            buttonTex = GameRegistry::getResource("optionsbtn-sheet.png",ResourceType::Texture).texture;
            break;
        case 3: // Credits
            buttonTex = GameRegistry::getResource("creditsbtn-sheet.png",ResourceType::Texture).texture;
            break;
        case 4: // Exit
            buttonTex = GameRegistry::getResource("exitbtn-sheet.png",ResourceType::Texture).texture;
            break;
        case 5: // Back
            buttonTex = GameRegistry::getResource("backbtn-sheet.png",ResourceType::Texture).texture;
            break;
        case 6: // Connect
            buttonTex = GameRegistry::getResource("connectbtn-sheet.png",ResourceType::Texture).texture;
            break;
        case 7: // Ready
            buttonTex = GameRegistry::getResource("readybtn.png",ResourceType::Texture).texture;
            break;
        case 8: // Emeraldo
            buttonTex = GameRegistry::getResource("emeraldo-pick.png",ResourceType::Texture).texture;
            break;
        case 9: // Rubie
            buttonTex = GameRegistry::getResource("rubie-pick.png",ResourceType::Texture).texture;
            break;
        case 10: // Sapheer
            buttonTex = GameRegistry::getResource("sapheer-pick.png",ResourceType::Texture).texture;
            break;
        default:
            std::cout << "GUIBUTTON FUNCTIONALITY UNKNOWN " << functionality << std::endl;
    }
    sf::Vector2f size = (sf::Vector2f)buttonTex->getSize();
    size.x/=2.f;
    shape.setSize(size);
    shape.setTexture(buttonTex);
    shape.setOrigin(size/2.f);
    shape.setScale(4.f,4.f);
    highlight(false);
}

void GuiButton::highlight( bool hightlightOn ){
    int halfWidth = shape.getTexture()->getSize().x/2;
    int height = shape.getTexture()->getSize().y;
    if ( hightlightOn ){
        shape.setTextureRect(sf::IntRect(sf::Vector2i(halfWidth,0),sf::Vector2i(halfWidth,height)));
    }
    else{
        shape.setTextureRect(sf::IntRect(sf::Vector2i(0,0),sf::Vector2i(halfWidth,height)));
    }
}

void GuiButton::setNext(GuiButton* button){
    next = button;
}

void GuiButton::setLast(GuiButton* button){
    last = button;
}

void GuiButton::setPosition(float x, float y){
    shape.setPosition(x,y);
}

void GuiButton::setPosition(sf::Vector2f position){
    shape.setPosition(position);
}

bool GuiButton::checkClick(sf::Event::MouseButtonEvent buttonEvent){
    if ( shape.getGlobalBounds().contains(buttonEvent.x,buttonEvent.y) ){
        switch( functionality ){
            case 1: // Play
                StageManager::pushStage(StageManager::connectMenu);
                break;
            case 2: // Options

                break;
            case 3: // Credits
                StageManager::pushStage(StageManager::creditsMenu);
                break;
            case 4: // Exit
                Display::window->close();
                break;
            case 5: // Back
                StageManager::popStage();
                break;
            case 6: // Connect
                //SyncManager::connectToServer(StageManager::connectMenu->field->getString());
                //StageManager::pushStage(StageManager::pickMenu);
                StageManager::popStage();
                StageManager::pushStage(StageManager::gameState);
                SyncManager::nickname = StageManager::connectMenu->nameField->getString();
                SyncManager::connectToServer(sf::IpAddress(StageManager::connectMenu->ipField->getString()));
                break;
            case 7: // Ready

                break;
            case 8: // Emeraldo
                //SyncManager::sendCrystalType(1);
                //StageManager::popStage();
                //StageManager::pushStage(StageManager::gameState);
                break;
            case 9: // Rubie
                //SyncManager::sendCrystalType(2);
                //StageManager::popStage();
                //StageManager::pushStage(StageManager::gameState);
                break;
            case 10: // Sapheer
                //SyncManager::sendCrystalType(3);
                //StageManager::popStage();
                //StageManager::pushStage(StageManager::gameState);
                break;
            default:
                std::cout << "GUIBUTTON FUNCTIONALITY UNKNOWN " << functionality << std::endl;
        }
        return true;
    }
    return false;
}

bool GuiButton::checkHover(sf::Event::MouseMoveEvent moveEvent){
    highlight(false);
    if ( shape.getGlobalBounds().contains(moveEvent.x,moveEvent.y) ){
        highlight(true);
        return true;
    }
    return false;
}

void GuiButton::draw(){
    Display::window->draw(shape);
}
