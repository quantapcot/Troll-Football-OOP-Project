#pragma once

#include "core/GameObject.h"

class Goal : public GameObject
{
public:
    Goal(float x);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::RectangleShape shape;
};