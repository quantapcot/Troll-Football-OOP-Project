#include "entities/Ground.h"
#include "core/GameConfig.h"

Ground::Ground()
{
    shape.setSize({
    Config::WINDOW_WIDTH,
    Config::GROUND_HEIGHT
        });

    shape.setFillColor(sf::Color(80, 180, 80));

    shape.setPosition({
    0.f,
    Config::GROUND_Y
        });
}

void Ground::update(float deltaTime)
{
    // Hiện tại Ground không cần cập nhật
}

void Ground::render(sf::RenderWindow& window)
{
    
}