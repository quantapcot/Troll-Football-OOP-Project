#pragma once

#include "core/GameObject.h"
#include "core/GameConfig.h"

#include <vector>
#include <optional>

struct ControlScheme
{
    sf::Keyboard::Key left;
    sf::Keyboard::Key right;
    sf::Keyboard::Key jump;
    sf::Keyboard::Key kick;
};

class Player : public GameObject
{
public:
    Player(const ControlScheme& controls,
        const sf::Color& color);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    void reset(const sf::Vector2f& pos)
    {
        position = pos;
        velocity = { 0.f,0.f };
        onGround = true;

        if (sprite)
            sprite->setPosition(position);
    }

    void setVelocity(const sf::Vector2f& v)
    {
        velocity = v;
    }

    sf::Vector2f getVelocity() const
    {
        return velocity;
    }

    void addPosition(const sf::Vector2f& offset)
    {
        position += offset;
    }

    const ControlScheme& getControls() const
    {
        return controls;
    }

    bool isFacingRight() const
    {
        return facingRight;
    }

    // =========================
    // KICK
    // =========================

    bool isKicking() const
    {
        return kicking;
    }

    bool hasKickHit() const
    {
        return kickHit;
    }

    void markKickHit()
    {
        kickHit = true;
    }

    sf::FloatRect getKickHitbox() const;

private:

    // =========================
    // PLAYER LOGIC
    // =========================

    void updateKick(float deltaTime);
    void updateDash(float deltaTime);

    void handleMovementInput();
    void handleDashInput();
    void handleKickInput();
    void handleJumpInput();

    void applyGravity(float deltaTime);
    void move(float deltaTime);

    void handleGroundCollision();
    void handleWallCollision();

    // ===== THÊM =====
    void updateTrail(float deltaTime);

    // =========================
    // COMPONENT
    // =========================

    std::optional<sf::Sprite> sprite;
    sf::Color playerColor;

    // =========================
    // MOVEMENT
    // =========================

    sf::Vector2f velocity{ 0.f, 0.f };

    float moveSpeed{ Config::PLAYER_SPEED };
    float jumpForce{ Config::PLAYER_JUMP_FORCE };
    float gravity{ Config::GRAVITY };

    bool onGround{ false };
    bool facingRight{ true };
    // =========================
    // KICK
    // =========================

    bool kicking{ false };
    bool kickHit{ false };

    float kickTimer{ 0.f };
    float kickDuration{ 0.18f };

    float kickCooldown{ 0.f };
    float kickCooldownTime{ 0.20f };

    // =========================
    // DASH
    // =========================

    bool dashing{ false };

    int dashDirection{ 0 };

    float dashTimer{ 0.f };
    float dashDuration{ 0.15f };

    float dashSpeed{ 600.f };

    float doubleTapWindow{ 0.25f };

    float leftTapTimer{ 0.f };
    float rightTapTimer{ 0.f };

    bool waitingSecondLeft{ false };
    bool waitingSecondRight{ false };

    bool lastAPressed{ false };
    bool lastDPressed{ false };

    // =========================
    // TRAIL
    // =========================

    struct Trail
    {
        sf::Vector2f position;
        float life;
    };

    std::vector<Trail> trails;

    float trailSpawnTimer{ 0.f };
    float trailSpawnInterval{ 0.025f };

    // =========================
    // CONTROLS
    // =========================

    ControlScheme controls;
};