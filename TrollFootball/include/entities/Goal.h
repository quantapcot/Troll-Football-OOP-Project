#pragma once

#include <SFML/Graphics.hpp>
#include "core/GameObject.h"
#include <optional>

class Ball;

class Goal : public GameObject
{
public:

    Goal(float x,
        bool isRightSide,
        const sf::Texture& texture);

    void update(float deltaTime) override;

    void render(sf::RenderWindow& window) override;

    bool contains(const Ball& ball) const;

    void handleCollision(Ball& ball);

private:

    // Trigger ghi bàn
    sf::RectangleShape goalTrigger;

    // Xà ngang
    sf::RectangleShape crossbar;

    // Chặn bóng bay ra sau Goal
    sf::RectangleShape backWall;

    // Hình ảnh Goal
    std::optional<sf::Sprite> sprite;
};