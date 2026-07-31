#include <iostream>
#include "core/Game.h"
#include "core/GameConfig.h"
#include "physics/Collision.h"

Game::Game()
    : window(
        sf::VideoMode(
            {
                static_cast<unsigned>(Config::WINDOW_WIDTH),
                static_cast<unsigned>(Config::WINDOW_HEIGHT)
            }),
        "TrollFootball")
{
    window.setFramerateLimit(60);

    // =========================
    // PLAYER 1
    // =========================

    ControlScheme p1Controls{
        sf::Keyboard::Key::A,
        sf::Keyboard::Key::D,
        sf::Keyboard::Key::Space,
        sf::Keyboard::Key::J
    };

    // =========================
    // PLAYER 2
    // =========================

    ControlScheme p2Controls{
        sf::Keyboard::Key::Left,
        sf::Keyboard::Key::Right,
        sf::Keyboard::Key::Up,
        sf::Keyboard::Key::Numpad0
    };

    player1 = std::make_unique<Player>(p1Controls, sf::Color::Red);
    player2 = std::make_unique<Player>(p2Controls, sf::Color::Blue);

    player1->reset({
        150.f,
        Config::GROUND_Y - Config::PLAYER_HALF_HEIGHT
        });

    player2->reset({
        Config::WINDOW_WIDTH - 150.f,
        Config::GROUND_Y - Config::PLAYER_HALF_HEIGHT
        });

    ball = std::make_unique<Ball>();
    ground = std::make_unique<Ground>();

    leftGoal = std::make_unique<Goal>(0.f);

    rightGoal = std::make_unique<Goal>(
        Config::WINDOW_WIDTH - Config::GOAL_WIDTH);
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
    player1->update(deltaTime);
    player2->update(deltaTime);

    ball->update(deltaTime);
    ground->update(deltaTime);

    leftGoal->update(deltaTime);
    rightGoal->update(deltaTime);

    // =========================
    // COLLISION PLAYER 1
    // =========================

    Collision::handlePlayerBall(*player1, *ball);
    Collision::handleKick(*player1, *ball);

    // =========================
    // COLLISION PLAYER 2
    // =========================

    Collision::handlePlayerBall(*player2, *ball);
    Collision::handleKick(*player2, *ball);

    // =========================
    // COLLISION PLAYER 1 VS PLAYER 2
    // =========================

    Collision::handlePlayerPlayer(*player1, *player2);

    // =========================
    // GOAL
    // =========================

    if (leftGoal->contains(*ball))
    {
        std::cout << "Right Player Scores!\n";

        resetAfterGoal();
        return;
    }

    if (rightGoal->contains(*ball))
    {
        std::cout << "Left Player Scores!\n";

        resetAfterGoal();
        return;
    }
}

void Game::render()
{
    window.clear(sf::Color(30, 120, 30));

    ground->render(window);

    leftGoal->render(window);
    rightGoal->render(window);

    ball->render(window);

    player1->render(window);
    player2->render(window);

    window.display();
}

void Game::resetAfterGoal()
{
    player1->reset({
        150.f,
        Config::GROUND_Y - Config::PLAYER_HALF_HEIGHT
        });

    player2->reset({
        Config::WINDOW_WIDTH - 150.f,
        Config::GROUND_Y - Config::PLAYER_HALF_HEIGHT
        });

    ball->reset({
        Config::WINDOW_WIDTH / 2.f,
        200.f
        });
}