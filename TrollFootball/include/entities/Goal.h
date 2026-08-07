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
private:

    //-------------------------
    // VISUAL
    //-------------------------

    std::optional<sf::Sprite> sprite;

    sf::Vector2f spritePosition;

    //-------------------------
    // PHYSICS
    //-------------------------

    sf::RectangleShape goalTrigger;
    sf::RectangleShape crossbar;
    sf::RectangleShape backWall;

    sf::Vector2f triggerPosition;
    sf::Vector2f crossbarPosition;
    sf::Vector2f backWallPosition;
};