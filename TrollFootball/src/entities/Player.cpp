#include "entities/Player.h"

Player::Player()
{
    shape.setSize({ 50.f, 70.f });
    shape.setOrigin({ 25.f, 35.f });
    shape.setFillColor(sf::Color::Red);

    // Vị trí ban đầu (cao hơn mặt đất để thấy hiệu ứng rơi)
    position = { 100.f, 650.f };

    shape.setPosition(position);
}

void Player::update(float deltaTime)
{
    // =========================
    // INPUT
    // =========================
    float direction = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        direction -= 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        direction += 1.f;

    velocity.x = direction * moveSpeed;

    // =========================
    // JUMP
    // =========================
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && onGround)
    {
        velocity.y = jumpForce;
        onGround = false;
    }

    // =========================
    // GRAVITY
    // =========================
    velocity.y += gravity * deltaTime;

    // =========================
    // UPDATE POSITION
    // =========================
    position += velocity * deltaTime;

    // =========================
    // GIỚI HẠN TRONG MÀN HÌNH
    // =========================
    const float WINDOW_WIDTH = 1024.f;
    const float WINDOW_HEIGHT = 768.f;

    const float PLAYER_HALF_WIDTH = 25.f;
    const float PLAYER_HALF_HEIGHT = 35.f;

    // Không cho đi ra bên trái
    if (position.x < PLAYER_HALF_WIDTH)
    {
        position.x = PLAYER_HALF_WIDTH;
    }

    // Không cho đi ra bên phải
    if (position.x > WINDOW_WIDTH - PLAYER_HALF_WIDTH)
    {
        position.x = WINDOW_WIDTH - PLAYER_HALF_WIDTH;
    }

    // =========================
    // GROUND COLLISION
    // =========================
    const float groundY = WINDOW_HEIGHT - PLAYER_HALF_HEIGHT;

    if (position.y >= groundY)
    {
        position.y = groundY;
        velocity.y = 0.f;
        onGround = true;
    }

    // =========================
    // UPDATE GRAPHICS
    // =========================
    shape.setPosition(position);
}

void Player::render(sf::RenderWindow& window)
{
    window.draw(shape);
}