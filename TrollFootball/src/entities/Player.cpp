#include "entities/Player.h"
#include "core/GameConfig.h"

Player::Player()
{
    shape.setSize({
        Config::PLAYER_WIDTH,
        Config::PLAYER_HEIGHT
        });

    shape.setOrigin({
        Config::PLAYER_HALF_WIDTH,
        Config::PLAYER_HALF_HEIGHT
        });

    shape.setFillColor(sf::Color::Red);

    // Vị trí ban đầu
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
    // GROUND COLLISION
    // =========================
    const float groundY =
        Config::GROUND_Y - Config::PLAYER_HALF_HEIGHT;

    if (position.y >= groundY)
    {
        position.y = groundY;
        velocity.y = 0.f;
        onGround = true;
    }
    else
    {
        onGround = false;
    }

    // =========================
    // WALL COLLISION
    // =========================
    if (position.x < Config::PLAYER_HALF_WIDTH)
    {
        position.x = Config::PLAYER_HALF_WIDTH;
    }

    if (position.x > Config::WINDOW_WIDTH - Config::PLAYER_HALF_WIDTH)
    {
        position.x = Config::WINDOW_WIDTH - Config::PLAYER_HALF_WIDTH;
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