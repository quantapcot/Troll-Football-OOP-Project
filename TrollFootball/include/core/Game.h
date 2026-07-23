#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "entities/Player.h"
#include "entities/Ball.h"

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow window;

    std::unique_ptr<Player> player;
    std::unique_ptr<Ball> ball;
};