#include "entities/Wall.h"
#include "core/GameConfig.h"

Wall::Wall(float x, float y, float width, float height)
{
    shape.setSize({
        width,
        height
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
        y
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