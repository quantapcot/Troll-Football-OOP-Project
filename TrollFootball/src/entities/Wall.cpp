#include "entities/Wall.h"
#include "core/GameConfig.h"

Wall::Wall(float x, float y, float width, float height)
{
    shape.setSize({
        width,
        height
        });

    shape.setFillColor(
        sf::Color::Transparent);

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