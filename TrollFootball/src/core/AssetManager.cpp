#include "core/AssetManager.h"
#include <iostream>
#include <filesystem>

AssetManager& AssetManager::get()
{
    static AssetManager instance;
    return instance;
}

void AssetManager::loadTexture(const std::string& name,
    const std::string& filename)
{
    std::cout << "========================\n";
    std::cout << "Loading: " << filename << std::endl;

    std::cout << "Exists: "
        << std::filesystem::exists(filename)
        << std::endl;

    sf::Texture texture;

    if (!texture.loadFromFile(filename))
    {
        std::cout << "FAIL\n";
        return;
    }

    std::cout << "SUCCESS\n";

    textures[name] = texture;
}

sf::Texture& AssetManager::getTexture(const std::string& name)
{
    auto it = textures.find(name);

    if (it == textures.end())
    {
        std::cout << "Texture NOT FOUND: " << name << std::endl;
        throw std::runtime_error("Texture not found: " + name);
    }

    return it->second;
}