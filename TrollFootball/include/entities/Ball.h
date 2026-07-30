#pragma once

#include "core/GameObject.h"
#include "core/GameConfig.h"

class Ball : public GameObject
{
public:
    Ball();

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    // =========================
    // GETTER
    // =========================

    const sf::Vector2f& getPosition() const
    {
        return position;
    }

    float getRadius() const
    {
        return shape.getRadius();
    }

    // =========================
    // SETTER
    // =========================

    void setVelocity(const sf::Vector2f& v)
    {
        velocity = v;
    }

    const sf::Vector2f& getVelocity() const
    {
        return velocity;
    }

    void setPosition(const sf::Vector2f& pos)
    {
        position = pos;
        shape.setPosition(position);
    }

    void reset(const sf::Vector2f& pos)
    {
        position = pos;
        velocity = { 0.f, 0.f };
        shape.setPosition(position);
    }

private:
    sf::CircleShape shape;

    sf::Vector2f velocity{ 0.f, 0.f };
};