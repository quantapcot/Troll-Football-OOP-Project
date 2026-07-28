#pragma once

#include "core/GameObject.h"

class Ball;

class Goal : public GameObject
{
public:
    Goal(float x);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool contains(const Ball& ball) const;

private:
    sf::RectangleShape shape;
};