#include "entities/Ball.h"
#include <cmath>

Ball::Ball()
{
    shape.setRadius(15.f);
    shape.setOrigin({ 15.f, 15.f });
    shape.setFillColor(sf::Color::White);

    position = { 512.f, 200.f };

    shape.setPosition(position);
}

void Ball::update(float deltaTime)
{
    // =========================
    // CONSTANTS
    // =========================
    const float WINDOW_WIDTH = 1024.f;
    const float WINDOW_HEIGHT = 768.f;

    const float BALL_RADIUS = 15.f;

    const float gravity = 1200.f;

    const float groundY = WINDOW_HEIGHT - BALL_RADIUS;

    const float bounce = 0.65f;

    // =========================
    // GRAVITY
    // =========================
    velocity.y += gravity * deltaTime;

    // =========================
    // UPDATE POSITION
    // =========================
    position += velocity * deltaTime;

    // =========================
    // TƯỜNG TRÁI / PHẢI
    // =========================
    if (position.x < BALL_RADIUS)
    {
        position.x = BALL_RADIUS;
        velocity.x *= -bounce;
    }

    if (position.x > WINDOW_WIDTH - BALL_RADIUS)
    {
        position.x = WINDOW_WIDTH - BALL_RADIUS;
        velocity.x *= -bounce;
    }

    // =========================
    // MẶT ĐẤT
    // =========================
    if (position.y >= groundY)
    {
        position.y = groundY;

        velocity.y *= -bounce;

        // Nếu nảy quá nhỏ thì dừng luôn
        if (std::abs(velocity.y) < 30.f)
            velocity.y = 0.f;
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