#pragma once

#include "core/GameObject.h"
#include "core/GameConfig.h"

class Player : public GameObject
{
public:
    Player();

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    // ===== THÊM =====
    void reset(const sf::Vector2f& pos)
    {
        position = pos;
        velocity = { 0.f,0.f };
        onGround = true;
        shape.setPosition(position);
    }

    // ===== THÊM =====
    void setVelocity(const sf::Vector2f& v)
    {
        velocity = v;
    }

private:
    sf::RectangleShape shape;

    sf::Vector2f velocity{ 0.f, 0.f };

    float moveSpeed{ Config::PLAYER_SPEED };
    float jumpForce{ Config::PLAYER_JUMP_FORCE };
    float gravity{ Config::GRAVITY };

    bool onGround{ false };
};