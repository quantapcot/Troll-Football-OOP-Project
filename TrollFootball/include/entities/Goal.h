#pragma once

#include <SFML/Graphics.hpp>
#include "core/GameObject.h"
#include <optional>

class Ball;

class Goal : public GameObject
{
public:
    Goal(float x, bool isRightSide, const sf::Texture& texture);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool contains(const Ball& ball) const;

private:
    sf::RectangleShape shape;
    std::optional<sf::Sprite> sprite;
};