#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

#include "core/GameObject.h"
#include "core/GameConfig.h"
#include "core/AssetManager.h"

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

    const sf::Vector2f& getVelocity() const
    {
        return velocity;
    }

    // =========================
    // SETTER
    // =========================

    void setVelocity(const sf::Vector2f& v)
    {
        velocity = v;
    }

    void addVelocity(const sf::Vector2f& impulse)
    {
        velocity += impulse;
    }

    void setPosition(const sf::Vector2f& pos)
    {
        position = pos;
        shape.setPosition(position);

        if (sprite)
            sprite->setPosition(position);
    }

    void addPosition(const sf::Vector2f& offset)
    {
        position += offset;

        shape.setPosition(position);

        if (sprite)
            sprite->setPosition(position);
    }

    void reset(const sf::Vector2f& pos)
    {
        position = pos;
        velocity = { 0.f, 0.f };

        // Reset góc xoay
        rotation = 0.f;

        shape.setPosition(position);

        if (sprite)
        {
            sprite->setPosition(position);

            // Đưa sprite về góc 0°
            sprite->setRotation(
                sf::degrees(0.f)
            );
        }
    }

private:
    // Collision vẫn dùng CircleShape
    sf::CircleShape shape;

    // Hình ảnh quả bóng
    std::optional<sf::Sprite> sprite;

    // Vận tốc
    sf::Vector2f velocity{ 0.f, 0.f };

    // Góc xoay hiện tại của bóng
    float rotation{ 0.f };

    // Hệ số chuyển vận tốc ngang -> tốc độ xoay
    float rotationFactor{ Config::BALL_ROTATION_FACTOR };
};