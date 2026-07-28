#include "entities/Goal.h"
#include "core/GameConfig.h"

Goal::Goal(float x)
{
    shape.setSize({
        Config::GOAL_WIDTH,
        Config::GOAL_HEIGHT
        });

    shape.setFillColor(sf::Color::White);

    shape.setPosition({
        x,
        Config::GROUND_Y - Config::GOAL_HEIGHT
        });
}

void Goal::update(float deltaTime)
{
    // Hiện tại chưa cần xử lý gì
}

void Goal::render(sf::RenderWindow& window)
{
    window.draw(shape);
}