#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "entities/Player.h"
#include "entities/Ball.h"
#include "entities/Ground.h"
#include "entities/Goal.h"

#include <optional>

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    void resetAfterGoal();

    sf::RenderWindow window;

    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    std::unique_ptr<Ball> ball;
    std::unique_ptr<Ground> ground;

    std::unique_ptr<Goal> leftGoal;
    std::unique_ptr<Goal> rightGoal;

    // =========================
    // SCORE
    // =========================

    int leftScore{ 0 };
    int rightScore{ 0 };

    std::optional<sf::Font> font;
    std::optional<sf::Text> scoreText;
};