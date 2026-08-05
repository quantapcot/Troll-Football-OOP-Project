#include "entities/Player.h"
#include "core/GameConfig.h"
#include "audio/AudioManager.h"
#include <algorithm>
#include <cstdint>
#include "core/AssetManager.h"
#include <iostream>

Player::Player(const ControlScheme& controls,
    const sf::Color& color)
    : controls(controls),
    playerColor(color)
{

    facingRight = (playerColor == sf::Color::Red);

    // Chọn texture theo Player
    if (playerColor == sf::Color::Red)
    {
        sprite.emplace(
            AssetManager::get().getTexture("player1"));
    }
    else
    {
        sprite.emplace(
            AssetManager::get().getTexture("player2"));
    }

    // Lấy kích thước ảnh
    auto size = sprite->getTexture().getSize();
    std::cout << "Player texture size: "
        << size.x << " x "
        << size.y << std::endl;

    // Đặt tâm Sprite
    sprite->setOrigin({
        size.x / 2.f,
        size.y / 2.f
        });

    // Scale về đúng kích thước nhân vật trong game
    sprite->setScale({ 0.18f, 0.18f });

    // Vị trí ban đầu
    position = {
        100.f,
        Config::GROUND_Y - Config::PLAYER_HALF_HEIGHT
    };

    sprite->setPosition(position);
}

void Player::update(float deltaTime)
{
    if (!aiControlled)
    {
        currentInput.left =
            sf::Keyboard::isKeyPressed(controls.left);

        currentInput.right =
            sf::Keyboard::isKeyPressed(controls.right);

        currentInput.jump =
            sf::Keyboard::isKeyPressed(controls.jump);

        currentInput.kick =
            sf::Keyboard::isKeyPressed(controls.kick);

        currentInput.dash =
            sf::Keyboard::isKeyPressed(controls.dash);
    }

    updateKick(deltaTime);
    updateDash(deltaTime);

    handleDashInput();

    handleMovementInput();
    handleKickInput();
    handleJumpInput();

    applyGravity(deltaTime);

    move(deltaTime);

    updateTrail(deltaTime);

    handleGroundCollision();
    handleWallCollision();

    sprite->setPosition(position);
}

void Player::updateKick(float deltaTime)
{
    if (kickCooldown > 0.f)
    {
        kickCooldown -= deltaTime;
    }

    if (kicking)
    {
        kickTimer -= deltaTime;

        if (kickTimer <= 0.f)
        {
            kicking = false;
            kickHit = false;
        }
    }
}

void Player::updateDash(float deltaTime)
{
    // Giảm cooldown
    if (dashCooldown > 0.f)
    {
        dashCooldown -= deltaTime;
    }

    // Dash đang diễn ra
    if (dashing)
    {
        dashTimer -= deltaTime;

        if (dashTimer <= 0.f)
        {
            dashing = false;
        }
    }
}

void Player::updateTrail(float deltaTime)
{
    // Giảm thời gian sống của trail
    for (auto& trail : trails)
    {
        trail.life -= deltaTime;
    }

    // Xóa trail đã hết thời gian
    trails.erase(
        std::remove_if(
            trails.begin(),
            trails.end(),
            [](const Trail& t)
            {
                return t.life <= 0.f;
            }),
        trails.end());

    // Chỉ tạo trail khi đang dash
    if (!dashing)
    {
        trailSpawnTimer = 0.f;
        return;
    }

    trailSpawnTimer += deltaTime;

    if (trailSpawnTimer >= trailSpawnInterval)
    {
        trailSpawnTimer = 0.f;

        trails.push_back(
            {
                position,
                0.15f
            });
    }
}

void Player::handleMovementInput()
{
    float direction = 0.f;

    if (currentInput.left)
        direction -= 1.f;

    if (currentInput.right)
        direction += 1.f;

    if (dashing)
        velocity.x = dashDirection * dashSpeed;
    else
        velocity.x = direction * moveSpeed;
}

void Player::handleKickInput()
{
    if (currentInput.kick)
    {
        if (!kicking && kickCooldown <= 0.f)
        {
            kicking = true;
            kickHit = false;
            kickTimer = kickDuration;
            kickCooldown = kickCooldownTime;
        }
    }
}

void Player::handleDashInput()
{
    if (dashing)
        return;

    if (dashCooldown > 0.f)
        return;

    if (!currentInput.dash)
        return;

    dashing = true;
    dashTimer = dashDuration;

    // Bắt đầu hồi chiêu
    dashCooldown = dashCooldownTime;

    if (currentInput.left)
    {
        dashDirection = -1;
    }
    else if (currentInput.right)
    {
        dashDirection = 1;
    }
    else
    {
        // Nếu không bấm hướng thì dash theo hướng nhìn
        dashDirection = facingRight ? 1 : -1;
    }
}

void Player::handleJumpInput()
{
    if (currentInput.jump && onGround)
    {
        velocity.y = jumpForce;
        onGround = false;
        AudioManager::getInstance().playSound("jump");
    }
}

void Player::applyGravity(float deltaTime)
{
    velocity.y += gravity * deltaTime;
}

void Player::move(float deltaTime)
{
    position += velocity * deltaTime;
}

void Player::handleGroundCollision()
{
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
}

void Player::handleWallCollision()
{
    if (position.x < Config::PLAYER_HALF_WIDTH)
    {
        position.x = Config::PLAYER_HALF_WIDTH;
    }

    if (position.x > Config::WINDOW_WIDTH - Config::PLAYER_HALF_WIDTH)
    {
        position.x = Config::WINDOW_WIDTH - Config::PLAYER_HALF_WIDTH;
    }
}

void Player::render(sf::RenderWindow& window)
{
    // =========================
    // DASH TRAIL
    // =========================

    for (const auto& trail : trails)
    {
        sf::Sprite ghost(*sprite);

        ghost.setPosition(trail.position);

        auto alpha = static_cast<std::uint8_t>(
            255.f * (trail.life / 0.15f));

        sf::Color trailColor = playerColor;
        trailColor.a = alpha;

        ghost.setColor(trailColor);

        window.draw(ghost);
    }

    // =========================
    // PLAYER
    // =========================

    if (sprite)
    {
        window.draw(*sprite);
    }

#ifdef _DEBUG

    if (kicking)
    {
        sf::RectangleShape hitbox;

        auto rect = getKickHitbox();

        hitbox.setPosition(rect.position);
        hitbox.setSize(rect.size);

        hitbox.setFillColor(
            sf::Color(255, 255, 0, 80));

        window.draw(hitbox);
    }

#endif
}

sf::FloatRect Player::getKickHitbox() const
{
    if (!kicking)
        return sf::FloatRect();

    constexpr float width = 45.f;
    constexpr float height = 50.f;

    float x;

    if (facingRight)
    {
        x = position.x + Config::PLAYER_HALF_WIDTH;
    }
    else
    {
        x = position.x - Config::PLAYER_HALF_WIDTH - width;
    }

    return sf::FloatRect(
        {
            x,
            position.y - Config::PLAYER_HALF_HEIGHT + 10.f
        },
        {
            width,
            height
        });
}