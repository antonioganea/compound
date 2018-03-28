#ifndef GAMEREGISTRY_H
#define GAMEREGISTRY_H

#include <map>
#include <stack>
#include <memory>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

enum class ResourceType
{
    Texture, SoundBuffer, Font
};

union ResourcePtr
{
    sf::Texture * texture;
    sf::Font * font;
    sf::SoundBuffer * soundBuffer;
};

struct ResourceCommand
{
    std::string filePath;
    ResourceType type;
};

class GameRegistry
{
    public:
        static void init();
        static bool queueResource( const std::string& filePath, const ResourceType& type );
        static bool loadResource();
        static ResourcePtr getResource( const std::string& filePath, const ResourceType& type );
    protected:

    private:
        GameRegistry();
        static std::map<std::string, ResourcePtr> m_resources;
        static std::stack<ResourceCommand> m_commands;
};

#endif // GAMEREGISTRY_H
