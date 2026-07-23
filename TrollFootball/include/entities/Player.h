#pragma once
#include "core/GameObject.h"

class Player : public GameObject
{
public:
    Player();

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    const sf::Vector2f& getPosition() const
    {
        return position;
    }

    sf::Vector2f getSize() const
    {
        return shape.getSize();
    }

private:
    sf::RectangleShape shape;

    // Vận tốc hiện tại của Player
    sf::Vector2f velocity{ 0.f, 0.f };

    // Thông số di chuyển
    float moveSpeed{ 200.f };
    float jumpForce{ -650.f };   // Âm để nhảy lên
    float gravity{ 1200.f };

    // Kiểm tra đang đứng trên mặt đất hay không
    bool onGround{ false };
};