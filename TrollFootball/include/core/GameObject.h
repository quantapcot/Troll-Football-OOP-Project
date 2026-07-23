#pragma once
#include <SFML/Graphics.hpp>

class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    const sf::Vector2f& getPosition() const { return position; }
    void setPosition(const sf::Vector2f& pos) { position = pos; }

protected:
    sf::Vector2f position{ 0.f, 0.f };
};