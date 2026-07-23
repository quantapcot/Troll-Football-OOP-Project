#pragma once

#include "core/GameObject.h"

class Ball : public GameObject
{
public:
    Ball();

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    // ===== Getter =====
    const sf::Vector2f& getPosition() const
    {
        return position;
    }

    float getRadius() const
    {
        return shape.getRadius();
    }

    // ===== Setter =====
    void setVelocity(const sf::Vector2f& v)
    {
        velocity = v;
    }

private:
    sf::CircleShape shape;

    sf::Vector2f velocity{ 0.f,0.f };
};