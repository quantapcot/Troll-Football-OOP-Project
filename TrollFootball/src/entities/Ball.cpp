#include "entities/Ball.h"
#include "core/GameConfig.h"
#include "core/AssetManager.h"
#include <cmath>
#include <iostream>

Ball::Ball()
{
    // Collision vẫn dùng bán kính
    shape.setRadius(Config::BALL_RADIUS);

    shape.setOrigin({
        Config::BALL_RADIUS,
        Config::BALL_RADIUS
        });

    // Load sprite
    sprite.emplace(
        AssetManager::get().getTexture("ball"));

    auto size = sprite->getTexture().getSize();

    std::cout << "Ball texture: "
        << size.x << " x "
        << size.y << std::endl;

    sprite->setOrigin({
        size.x / 2.f,
        size.y / 2.f
        });

    // Scale tạm, lát sẽ chỉnh nếu cần
    sprite->setScale({ 0.08f, 0.08f });

    sprite->setRotation(
        sf::degrees(0.f)
    );

    position =
    {
        Config::WINDOW_WIDTH / 2.f,
        200.f
    };

    sprite->setPosition(position);
}

void Ball::update(float deltaTime)
{
    // =========================
    // GRAVITY
    // =========================

    velocity.y += Config::GRAVITY * deltaTime;

    // =========================
    // AIR DRAG
    // =========================

    velocity.x *= 0.998f;
    velocity.y *= 0.9995f;

    // =========================
    // UPDATE POSITION
    // =========================

    position += velocity * deltaTime;

    // =========================
    // WALL COLLISION
    // =========================

    if (position.x < Config::BALL_RADIUS)
    {
        position.x = Config::BALL_RADIUS;
        velocity.x *= -Config::BALL_BOUNCE;
    }

    if (position.x > Config::WINDOW_WIDTH - Config::BALL_RADIUS)
    {
        position.x = Config::WINDOW_WIDTH - Config::BALL_RADIUS;
        velocity.x *= -Config::BALL_BOUNCE;
    }

    // =========================
    // GROUND COLLISION
    // =========================

    const float groundY =
        Config::GROUND_Y - Config::BALL_RADIUS;

    if (position.y >= groundY)
    {
        position.y = groundY;

        velocity.y *= -(Config::BALL_BOUNCE * 0.92f);

        if (std::abs(velocity.y) < 30.f)
        {
            velocity.y = 0.f;
            // Ma sát khi bóng nằm trên mặt đất
            velocity.x *= 0.985f;
        }
    }

    // =========================
    // STOP SMALL HORIZONTAL SPEED
    // =========================

    if (std::abs(velocity.x) < 5.f)
    {
        velocity.x = 0.f;
    }

    // =========================
    // UPDATE GRAPHICS
    // =========================

    shape.setPosition(position);

    if (sprite)
    {
        sprite->setPosition(position);

        // =========================
        // ROTATION
        // =========================

        rotation += velocity.x * rotationFactor * deltaTime;

        sprite->setRotation(
            sf::degrees(rotation)
        );
    }
}

void Ball::render(sf::RenderWindow& window)
{
    if (sprite)
        window.draw(*sprite);
    else
        window.draw(shape);
}