#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class AssetManager
{
public:
    static AssetManager& get();

    void loadTexture(const std::string& name,
        const std::string& filename);

    sf::Texture& getTexture(const std::string& name);

private:
    std::unordered_map<std::string, sf::Texture> textures;
};