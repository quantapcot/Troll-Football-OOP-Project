#pragma once
#include <SFML/Graphics.hpp>

//Interface truu tuong IScreen
class IScreen
{
public:
    virtual ~IScreen() = default;

    virtual void draw(sf::RenderWindow& window) = 0;
};