#include "entities/Ball.h"
#include "core/GameConfig.h"
#include <cmath>

Ball::Ball()
{
    shape.setRadius(Config::BALL_RADIUS);

    shape.setOrigin({
        Config::BALL_RADIUS,
        Config::BALL_RADIUS
        });

    shape.setFillColor(sf::Color::White);

    position = {
        Config::WINDOW_WIDTH / 2.f,
        200.f
    };

    shape.setPosition(position);
}

void Ball::update(float deltaTime)
{
    // =========================
    // GRAVITY
    // =========================
    velocity.y += Config::GRAVITY * deltaTime;

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

        velocity.y *= -Config::BALL_BOUNCE;

        // Nếu nảy quá nhỏ thì dừng luôn
        if (std::abs(velocity.y) < 30.f)
        {
            velocity.y = 0.f;
        }
    }

    // =========================
    // UPDATE GRAPHICS
    // =========================
    shape.setPosition(position);
}

void Ball::render(sf::RenderWindow& window)
{
    window.draw(shape);
}