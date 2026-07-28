#pragma once

#include "core/GameObject.h"
#include "core/GameConfig.h"

class Player : public GameObject
{
public:
    Player();

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::RectangleShape shape;

    // Vận tốc hiện tại
    sf::Vector2f velocity{ 0.f, 0.f };

    // Các thông số của Player
    float moveSpeed{ Config::PLAYER_SPEED };
    float jumpForce{ Config::PLAYER_JUMP_FORCE };
    float gravity{ Config::GRAVITY };

    // Trạng thái
    bool onGround{ false };
};