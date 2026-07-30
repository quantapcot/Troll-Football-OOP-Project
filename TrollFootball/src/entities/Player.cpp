#include "entities/Player.h"
#include "core/GameConfig.h"
#include <algorithm>
#include <cstdint>

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

    position = { 100.f, 650.f };

    shape.setPosition(position);
}

void Player::update(float deltaTime)
{
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

    shape.setPosition(position);
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
    if (dashing)
    {
        dashTimer -= deltaTime;

        if (dashTimer <= 0.f)
        {
            dashing = false;
        }
    }

    if (waitingSecondLeft)
    {
        leftTapTimer -= deltaTime;

        if (leftTapTimer <= 0.f)
        {
            waitingSecondLeft = false;
        }
    }

    if (waitingSecondRight)
    {
        rightTapTimer -= deltaTime;

        if (rightTapTimer <= 0.f)
        {
            waitingSecondRight = false;
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        direction -= 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        direction += 1.f;

    if (dashing)
    {
        velocity.x = dashDirection * dashSpeed;
    }
    else
    {
        velocity.x = direction * moveSpeed;
    }
}

void Player::handleDashInput()
{
    bool currentA = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    bool currentD = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

    bool pressA = currentA && !lastAPressed;
    bool pressD = currentD && !lastDPressed;

    // =========================
    // DOUBLE TAP LEFT
    // =========================

    if (pressA)
    {
        if (waitingSecondLeft)
        {
            dashing = true;
            dashDirection = -1;
            dashTimer = dashDuration;

            waitingSecondLeft = false;
        }
        else
        {
            waitingSecondLeft = true;
            leftTapTimer = doubleTapWindow;
        }
    }

    // =========================
    // DOUBLE TAP RIGHT
    // =========================

    if (pressD)
    {
        if (waitingSecondRight)
        {
            dashing = true;
            dashDirection = 1;
            dashTimer = dashDuration;

            waitingSecondRight = false;
        }
        else
        {
            waitingSecondRight = true;
            rightTapTimer = doubleTapWindow;
        }
    }

    lastAPressed = currentA;
    lastDPressed = currentD;
}

void Player::handleKickInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J))
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

void Player::handleJumpInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && onGround)
    {
        velocity.y = jumpForce;
        onGround = false;
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
        sf::RectangleShape ghost = shape;

        ghost.setPosition(trail.position);

        auto alpha = static_cast<std::uint8_t>(
            255.f * (trail.life / 0.15f));

        ghost.setFillColor(
            sf::Color(
                255,
                0,
                0,
                alpha));

        window.draw(ghost);
    }

    // =========================
    // PLAYER
    // =========================

    window.draw(shape);

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

    return sf::FloatRect(
        {
            position.x + Config::PLAYER_HALF_WIDTH,
            position.y - Config::PLAYER_HALF_HEIGHT + 10.f
        },
        {
            45.f,
            50.f
        }
    );
}