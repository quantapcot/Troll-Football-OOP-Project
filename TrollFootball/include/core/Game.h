#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "entities/Player.h"
#include "entities/Ball.h"
#include "entities/Ground.h"
#include "entities/Goal.h"

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

    std::unique_ptr<Player> player;
    std::unique_ptr<Ball> ball;
    std::unique_ptr<Ground> ground;

    std::unique_ptr<Goal> leftGoal;
    std::unique_ptr<Goal> rightGoal;
};