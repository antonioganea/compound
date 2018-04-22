#include "GameRegistry.h"

#include "DebugLog.h"

#include <string>
#include <iostream>

std::map<std::string, ResourcePtr> GameRegistry::m_resources;
std::stack<ResourceCommand> GameRegistry::m_commands;

void GameRegistry::init(){
    //GameRegistry::queueResource("emeraldo.png",ResourceType::Texture);
    //GameRegistry::queueResource("shard_emeraldo.png",ResourceType::Texture);
    //GameRegistry::queueResource("emeraldo_small.png",ResourceType::Texture);
    //GameRegistry::queueResource("rubie.png",ResourceType::Texture);
    //GameRegistry::queueResource("shard_rubie.png",ResourceType::Texture);
    //GameRegistry::queueResource("laser.png",ResourceType::Texture);
    //GameRegistry::queueResource("sapheer.png",ResourceType::Texture);
    //GameRegistry::queueResource("shard_sapheer.png",ResourceType::Texture);
    //GameRegistry::queueResource("walls.png",ResourceType::Texture);

    GameRegistry::queueResource("emeraldo2.png",ResourceType::Texture);
    GameRegistry::queueResource("1.png",ResourceType::Texture);
    GameRegistry::queueResource("2.png",ResourceType::Texture);
    GameRegistry::queueResource("3.png",ResourceType::Texture);
    GameRegistry::queueResource("4.png",ResourceType::Texture);
    GameRegistry::queueResource("5.png",ResourceType::Texture);
    GameRegistry::queueResource("6.png",ResourceType::Texture);
    GameRegistry::queueResource("7.png",ResourceType::Texture);
    GameRegistry::queueResource("8.png",ResourceType::Texture);
    GameRegistry::queueResource("9.png",ResourceType::Texture);
    GameRegistry::queueResource("10.png",ResourceType::Texture);
    GameRegistry::queueResource("11.png",ResourceType::Texture);
    GameRegistry::queueResource("12.png",ResourceType::Texture);
    GameRegistry::queueResource("13.png",ResourceType::Texture);
    GameRegistry::queueResource("14.png",ResourceType::Texture);
    GameRegistry::queueResource("15.png",ResourceType::Texture);
    GameRegistry::queueResource("16.png",ResourceType::Texture);
    GameRegistry::queueResource("projectile.png",ResourceType::Texture);

    GameRegistry::queueResource("icepattern.png",ResourceType::Texture);
    GameRegistry::queueResource("playbtn-sheet.png",ResourceType::Texture);
    GameRegistry::queueResource("optionsbtn-sheet.png",ResourceType::Texture);
    GameRegistry::queueResource("creditsbtn-sheet.png",ResourceType::Texture);
    GameRegistry::queueResource("exitbtn-sheet.png",ResourceType::Texture);
    GameRegistry::queueResource("backbtn-sheet.png",ResourceType::Texture);
    GameRegistry::queueResource("picktxt.png", ResourceType::Texture);
    GameRegistry::queueResource("arena.png", ResourceType::Texture);
    GameRegistry::queueResource("dpcomic.ttf",ResourceType::Font);
    GameRegistry::queueResource("ARCADECLASSIC.TTF",ResourceType::Font);
    GameRegistry::queueResource("connectbtn-sheet.png", ResourceType::Texture);
    GameRegistry::queueResource("readybtn-sheet.png", ResourceType::Texture);
    GameRegistry::queueResource("emeraldo-pick.png", ResourceType::Texture);
    GameRegistry::queueResource("rubie-pick.png", ResourceType::Texture);
    GameRegistry::queueResource("sapheer-pick.png", ResourceType::Texture);
    while (!GameRegistry::loadResource()){}
}

bool GameRegistry::queueResource( const std::string& filePath, const ResourceType& type  )
{
    ResourceCommand command;
    command.filePath = filePath;
    command.type = type;
    m_commands.push( command );
    DebugLog::print( LogLevel::Info, "Queued resource : %s", filePath.c_str() );
    return true;
}

std::string composeFullPath( const std::string& filePath, const ResourceType& type ){
    std::string fullPath;
    switch ( type ){
    case ResourceType::Texture:
        fullPath = "data/textures/" + filePath;
        break;
    case ResourceType::Font:
        fullPath = "data/fonts/" + filePath;
        break;
    case ResourceType::SoundBuffer:
        fullPath = "data/sounds/" + filePath;
        break;
    }
    DebugLog::print( LogLevel::Trace, "Composed fullPath : %s", fullPath.c_str() );
    return fullPath;
}

ResourcePtr GameRegistry::getResource( const std::string& filePath, const ResourceType& type ){
    std::string fullPath = composeFullPath( filePath, type );
    DebugLog::print( LogLevel::Trace, "Returning resource : %s", filePath.c_str() );
    return m_resources.find(fullPath)->second;//use full path
}

bool GameRegistry::loadResource()
{
    if ( m_commands.size() )
    {
        ResourceType type = m_commands.top().type;
        std::string fullPath = composeFullPath( m_commands.top().filePath, type );

        ResourcePtr res;

        switch ( type ){
            case ResourceType::Texture :{
                sf::Texture * texture = new sf::Texture;
                texture->loadFromFile(fullPath);
                res.texture = texture;
                DebugLog::print( LogLevel::Trace, "Loading texture : %s, size %dx%d", fullPath.c_str(), texture->getSize().x, texture->getSize().y );
                break;
            }
            case ResourceType::Font :{
                sf::Font * font = new sf::Font;
                font->loadFromFile(fullPath);
                res.font = font;
                DebugLog::print( LogLevel::Trace, "Loading font : %s", fullPath.c_str() );
                break;
            }
            case ResourceType::SoundBuffer :{
                sf::SoundBuffer * soundBuffer = new sf::SoundBuffer;
                soundBuffer->loadFromFile(fullPath);
                res.soundBuffer = soundBuffer;
                DebugLog::print( LogLevel::Trace, "Loading sound : %s", fullPath.c_str() );
                break;
            }
        };

        m_commands.pop();

        m_resources.insert(std::pair<std::string,ResourcePtr>(fullPath, res));

        return false;
    }
    else
        return true;
}
