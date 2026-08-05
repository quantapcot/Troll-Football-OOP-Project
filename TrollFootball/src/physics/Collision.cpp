#include "physics/Collision.h"
#include "audio/AudioManager.h"
#include "entities/Player.h"
#include "entities/Ball.h"
#include "core/GameConfig.h"

#include <cmath>

void Collision::handlePlayerBall(Player& player, Ball& ball)
{
    sf::Vector2f playerPos = player.getPosition();
    sf::Vector2f ballPos = ball.getPosition();

    float dx = playerPos.x - ballPos.x;
    float dy = playerPos.y - ballPos.y;

    float distance = std::sqrt(dx * dx + dy * dy);

    const float playerRadius = Config::PLAYER_COLLISION_RADIUS;
    const float ballRadius = ball.getRadius();

    bool touching =
        distance <= playerRadius + ballRadius;

    // Tránh chia cho 0
    if (distance <= 0.001f)
        return;

    // =========================
    // Không còn chạm
    // =========================

    if (!touching)
        return;

    // =========================
    // POSITIONAL CORRECTION
    // =========================

    float overlap =
        playerRadius + ballRadius - distance;

    sf::Vector2f normal(
        (ballPos.x - playerPos.x) / distance,
        (ballPos.y - playerPos.y) / distance
    );

    // Đẩy bóng ra khỏi Player
    ball.addPosition(normal * (overlap + 1.f));

    // =========================
    // Bump
    // =========================

    float bumpX =
        Config::PLAYER_BUMP_FORCE_X;

    // Player đang Dash thì húc mạnh hơn
    if (player.isDashing())
    {
        bumpX *= 1.6f;
    }

    // Cộng thêm vận tốc của Player
    float forceX =
        bumpX +
        player.getVelocity().x;

    float forceY =
        Config::PLAYER_BUMP_FORCE_Y;

    if (player.isDashing())
    {
        forceY *= 1.1f;
    }

    if (dx < 0)
    {
        ball.addVelocity({
            forceX,
            forceY
            });
    }
    else
    {
        ball.addVelocity({
            -forceX,
            forceY
            });
    }
}

void Collision::handleKick(Player& player, Ball& ball)
{
    if (!player.isKicking())
        return;

    if (player.hasKickHit())
        return;

    if (!player.getKickHitbox().contains(ball.getPosition()))
        return;

    if (player.isFacingRight())
    {
        ball.addVelocity({
            Config::PLAYER_KICK_FORCE_X,
            Config::PLAYER_KICK_FORCE_Y
            });
    }
    else
    {
        ball.addVelocity({
            -Config::PLAYER_KICK_FORCE_X,
            Config::PLAYER_KICK_FORCE_Y
            });
    }

	AudioManager::getInstance().playSound("kick");

    player.markKickHit();
}

void Collision::handlePlayerPlayer(Player& p1, Player& p2)
{
    sf::Vector2f pos1 = p1.getPosition();
    sf::Vector2f pos2 = p2.getPosition();

    float dx = pos2.x - pos1.x;
    float dy = pos2.y - pos1.y;

    float distance = std::sqrt(dx * dx + dy * dy);

    float minDistance =
        Config::PLAYER_COLLISION_RADIUS * 2.f;

    if (distance <= 0.f || distance >= minDistance)
        return;

    float overlap = minDistance - distance;

    sf::Vector2f normal(
        dx / distance,
        dy / distance);

    p1.addPosition(-normal * overlap * 0.5f);
    p2.addPosition(normal * overlap * 0.5f);

    sf::Vector2f v1 = p1.getVelocity();
    sf::Vector2f v2 = p2.getVelocity();

    std::swap(v1.x, v2.x);

    p1.setVelocity(v1);
    p2.setVelocity(v2);
}