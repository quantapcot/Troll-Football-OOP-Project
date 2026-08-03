#include "entities/Goal.h"
#include "core/GameConfig.h"
#include "entities/Ball.h"

Goal::Goal(float x)
{
    shape.setSize({
        Config::GOAL_WIDTH,
        Config::GOAL_HEIGHT
        });

    // Hitbox vô hình
    shape.setFillColor(sf::Color::Transparent);

    shape.setPosition({
        x,
        Config::GROUND_Y - Config::GOAL_HEIGHT
        });
}

void Goal::update(float deltaTime)
{
    // Chưa cần xử lý
}

void Goal::render(sf::RenderWindow& window)
{
    // Không vẽ gì nữa.
    // Muốn xem hitbox thì dùng:
    // window.draw(shape);
}

bool Goal::contains(const Ball& ball) const
{
    return shape.getGlobalBounds().contains(ball.getPosition());
}