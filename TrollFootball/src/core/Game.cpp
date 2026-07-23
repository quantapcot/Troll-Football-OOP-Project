#include "core/Game.h"
#include <cmath>

Game::Game()
    : window(sf::VideoMode({ 1024u, 768u }), "TrollFootball")
{
    window.setFramerateLimit(60);

    player = std::make_unique<Player>();
    ball = std::make_unique<Ball>();
}

void Game::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void Game::update(float deltaTime)
{
    player->update(deltaTime);
    ball->update(deltaTime);

    // =========================
    // PLAYER - BALL COLLISION
    // =========================

    sf::Vector2f playerPos = player->getPosition();
    sf::Vector2f ballPos = ball->getPosition();

    float dx = playerPos.x - ballPos.x;
    float dy = playerPos.y - ballPos.y;

    float distance = std::sqrt(dx * dx + dy * dy);

    const float playerRadius = 35.f;
    const float ballRadius = ball->getRadius();

    if (distance <= playerRadius + ballRadius)
    {
        if (dx < 0)
            ball->setVelocity({ 450.f,-250.f });
        else
            ball->setVelocity({ -450.f,-250.f });
    }
}

void Game::render()
{
    window.clear(sf::Color(30, 120, 30));

    player->render(window);
    ball->render(window);

    window.display();
}