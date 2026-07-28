#pragma once

#include "core/GameObject.h"

class Ground : public GameObject
{
public:
    Ground();

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::RectangleShape shape;
};