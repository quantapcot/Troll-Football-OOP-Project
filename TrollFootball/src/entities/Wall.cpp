#include "entities/Wall.h"
#include "core/GameConfig.h"

Wall::Wall(float x)
{
    shape.setSize({
        10.f,
        Config::WINDOW_HEIGHT
        });

#ifdef _DEBUG
    shape.setFillColor(
        sf::Color(255, 0, 255, 120));
#else
    shape.setFillColor(
        sf::Color::Transparent);
#endif

    shape.setPosition({
        x,
        0.f
        });
}

void Wall::update(float)
{
}

void Wall::render(sf::RenderWindow& window)
{
    window.draw(shape);
}

const sf::FloatRect Wall::getBounds() const
{
    return shape.getGlobalBounds();
}