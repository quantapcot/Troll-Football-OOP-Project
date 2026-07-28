#include "entities/Ground.h"

Ground::Ground()
{
    shape.setSize({ 1024.f, 35.f });

    shape.setFillColor(sf::Color(80, 180, 80));

    shape.setPosition({ 0.f, 733.f });
}

void Ground::update(float deltaTime)
{
    // Hiện tại Ground không cần cập nhật
}

void Ground::render(sf::RenderWindow& window)
{
    window.draw(shape);
}