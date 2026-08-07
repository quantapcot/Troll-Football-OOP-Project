#pragma once

#include <SFML/Graphics.hpp>
#include "core/GameObject.h"

class Wall : public GameObject
{
public:
    Wall(float x);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    const sf::FloatRect getBounds() const;

private:
    sf::RectangleShape shape;
};