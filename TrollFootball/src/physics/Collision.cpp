#include "physics/Collision.h"
#include "audio/AudioManager.h"
#include "entities/Player.h"
#include "entities/Ball.h"
#include "entities/Wall.h"
#include "core/GameConfig.h"
#include "entities/AsteroidManager.h"

#include <cmath>

void Collision::handlePlayerBall(Player& player, Ball& ball)
{
    // =========================
    // PLAYER BODY HITBOX
    // =========================

    sf::FloatRect playerRect =
        player.getBodyHitbox();

    sf::Vector2f ballPos =
        ball.getPosition();

    float radius =
        ball.getRadius();

    // =========================
    // TÌM ĐIỂM GẦN NHẤT
    // TRÊN RECTANGLE PLAYER
    // =========================

    float closestX =
        std::clamp(
            ballPos.x,
            playerRect.position.x,
            playerRect.position.x +
            playerRect.size.x
        );

    float closestY =
        std::clamp(
            ballPos.y,
            playerRect.position.y,
            playerRect.position.y +
            playerRect.size.y
        );

    float dx =
        ballPos.x - closestX;

    float dy =
        ballPos.y - closestY;

    float distanceSquared =
        dx * dx + dy * dy;

    // =========================
    // KHÔNG CHẠM
    // =========================

    if (distanceSquared >
        radius * radius)
    {
        return;
    }

    // =========================
    // TRƯỜNG HỢP TÂM BÓNG
    // NẰM BÊN TRONG PLAYER
    // =========================

    if (distanceSquared < 0.0001f)
    {
        float playerCenterX =
            playerRect.position.x +
            playerRect.size.x * 0.5f;

        float playerCenterY =
            playerRect.position.y +
            playerRect.size.y * 0.5f;

        float dirX =
            ballPos.x - playerCenterX;

        float dirY =
            ballPos.y - playerCenterY;

        // Chọn hướng đẩy theo phía gần nhất
        if (std::abs(dirX) > std::abs(dirY))
        {
            dx = (dirX >= 0.f) ? 1.f : -1.f;
            dy = 0.f;
        }
        else
        {
            dx = 0.f;
            dy = (dirY >= 0.f) ? 1.f : -1.f;
        }

        distanceSquared = 1.f;
    }

    float distance =
        std::sqrt(distanceSquared);

    // Normal từ Player -> Ball
    sf::Vector2f normal(
        dx / distance,
        dy / distance
    );

    // =========================
    // ĐẨY BÓNG RA KHỎI PLAYER
    // =========================

    float penetration =
        radius - distance;

    if (penetration > 0.f)
    {
        ball.addPosition(
            normal *
            (penetration + 1.f)
        );
    }

    // =========================
    // BUMP
    // =========================

    float bumpX =
        Config::PLAYER_BUMP_FORCE_X;

    if (player.isDashing())
    {
        bumpX *= 1.6f;
    }

    // Vận tốc Player truyền sang Ball
    sf::Vector2f playerVelocity =
        player.getVelocity();

    sf::Vector2f ballVelocity =
        ball.getVelocity();

    // =========================
    // CHỈ ĐẨY NẾU BÓNG ĐANG
    // ĐI VÀO PLAYER
    // =========================

    float relativeVelocity =
        (ballVelocity.x - playerVelocity.x) * normal.x +
        (ballVelocity.y - playerVelocity.y) * normal.y;

    if (relativeVelocity > 0.f)
    {
        return;
    }

    // =========================
    // BUMP THEO NORMAL
    // =========================

    float force =
        bumpX + std::abs(playerVelocity.x);

    if (player.isDashing())
    {
        force *= 1.2f;
    }

    sf::Vector2f newVelocity =
        ballVelocity;

    // Phản lực theo normal
    newVelocity.x +=
        normal.x * force;

    newVelocity.y +=
        normal.y * force;

    // Thêm một chút lực từ Player
    newVelocity.x +=
        playerVelocity.x * 0.35f;

    ball.setVelocity(newVelocity);
}

void Collision::handlePlayersBall(Player& p1, Player& p2, Ball& ball)
{
    sf::Vector2f ballPos = ball.getPosition();
    float radius = ball.getRadius();

    // Helper lambda để tính penetration của 1 Player
    auto calcOverlap = [&](Player& p, float& pen, sf::Vector2f& norm) {
        sf::FloatRect rect = p.getBodyHitbox();
        float closestX = std::clamp(ballPos.x, rect.position.x, rect.position.x + rect.size.x);
        float closestY = std::clamp(ballPos.y, rect.position.y, rect.position.y + rect.size.y);
        
        float dx = ballPos.x - closestX;
        float dy = ballPos.y - closestY;
        float distSq = dx * dx + dy * dy;

        if (distSq > radius * radius) {
            pen = 0.f;
            return;
        }

        if (distSq < 0.0001f) {
            float cX = rect.position.x + rect.size.x * 0.5f;
            float cY = rect.position.y + rect.size.y * 0.5f;
            float dirX = ballPos.x - cX;
            float dirY = ballPos.y - cY;
            if (std::abs(dirX) > std::abs(dirY)) {
                dx = (dirX >= 0.f) ? 1.f : -1.f;
                dy = 0.f;
            } else {
                dx = 0.f;
                dy = (dirY >= 0.f) ? 1.f : -1.f;
            }
            distSq = 1.f;
        }

        float dist = std::sqrt(distSq);
        norm = sf::Vector2f(dx / dist, dy / dist);
        pen = radius - dist;
    };

    float pen1 = 0.f, pen2 = 0.f;
    sf::Vector2f norm1, norm2;
    calcOverlap(p1, pen1, norm1);
    calcOverlap(p2, pen2, norm2);

    // KHI CẢ HAI ĐỀU ĐANG ĐÈ LÊN BÓNG
    if (pen1 > 0.f && pen2 > 0.f) {
        // Tính Dot product để xem 2 hướng đẩy bóng có ngược chiều nhau không
        float dot = norm1.x * norm2.x + norm1.y * norm2.y;
        
        if (dot < -0.5f) {
            // Bóng bị kẹp chặt giữa 2 player (ngược hướng nhau)
            // Áp dụng "Squirting": Đẩy bóng vọt lên trên (Trục Y âm)
            ball.addPosition(sf::Vector2f(0.f, -std::max(pen1, pen2) - 10.f));
            
            sf::Vector2f vel = ball.getVelocity();
            vel.y = std::min(vel.y, -350.f); // Cho bóng nảy lên trên một chút để thoát hẳn
            ball.setVelocity(vel);
            return;
        }
    }

    // Nếu không bị ép cứng, fallback xử lý độc lập từng người
    // (Bảo toàn 100% logic sút bóng/bump bóng cũ)
    if (pen1 > 0.f) handlePlayerBall(p1, ball);
    if (pen2 > 0.f) handlePlayerBall(p2, ball);
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
            player.getKickForceX(),
            player.getKickForceY()
            });
    }
    else
    {
        ball.addVelocity({
            -player.getKickForceX(),
            player.getKickForceY()
            });
    }

	AudioManager::getInstance().playSound("kick");

    player.markKickHit();
}

void Collision::handlePlayerPlayer(Player& p1, Player& p2)
{
    sf::FloatRect rect1 = p1.getBodyHitbox();
    sf::FloatRect rect2 = p2.getBodyHitbox();

    auto intersection = rect1.findIntersection(rect2);

    // Không chạm nhau
    if (!intersection.has_value())
        return;

    sf::FloatRect overlap = intersection.value();

    // =========================
    // XÁC ĐỊNH HƯỚNG ĐẨY
    // =========================

    float center1 =
        rect1.position.x + rect1.size.x * 0.5f;

    float center2 =
        rect2.position.x + rect2.size.x * 0.5f;

    // Hai player chồng lên nhau theo chiều X
    if (overlap.size.x < overlap.size.y)
    {
        // Đẩy theo chiều ngang
        if (center1 < center2)
        {
            p1.addPosition({
                -overlap.size.x * 0.5f,
                0.f
                });

            p2.addPosition({
                overlap.size.x * 0.5f,
                0.f
                });
        }
        else
        {
            p1.addPosition({
                overlap.size.x * 0.5f,
                0.f
                });

            p2.addPosition({
                -overlap.size.x * 0.5f,
                0.f
                });
        }
    }
    else
    {
        // =========================
        // CHỒNG LÊN NHAU THEO CHIỀU Y
        // =========================

        float centerY1 =
            rect1.position.y + rect1.size.y * 0.5f;

        float centerY2 =
            rect2.position.y + rect2.size.y * 0.5f;

        if (centerY1 < centerY2)
        {
            p1.addPosition({
                0.f,
                -overlap.size.y * 0.5f
                });

            p2.addPosition({
                0.f,
                overlap.size.y * 0.5f
                });
        }
        else
        {
            p1.addPosition({
                0.f,
                overlap.size.y * 0.5f
                });

            p2.addPosition({
                0.f,
                -overlap.size.y * 0.5f
                });
        }
    }

    // =========================
    // ĐỔI VẬN TỐC X
    // =========================

    sf::Vector2f v1 = p1.getVelocity();
    sf::Vector2f v2 = p2.getVelocity();

    std::swap(v1.x, v2.x);

    p1.setVelocity(v1);
    p2.setVelocity(v2);
}

void Collision::handleBallWall(
    Ball& ball,
    const Wall& wall)
{
    auto wallRect = wall.getBounds();

    sf::Vector2f pos = ball.getPosition();
    float r = ball.getRadius();

    sf::FloatRect ballRect(
        {
            pos.x - r,
            pos.y - r
        },
        {
            r * 2.f,
            r * 2.f
        });

        // Không va chạm
        if (!wallRect.findIntersection(ballRect))
        {
            return;
        }

        auto vel = ball.getVelocity();

        // =========================
        // WALL NGANG - TOP / BOTTOM
        // =========================
        if (wallRect.size.x > wallRect.size.y)
        {
            // Bóng đang bay lên -> đụng mặt dưới của top wall
            if (vel.y < 0.f)
            {
                pos.y =
                    wallRect.position.y +
                    wallRect.size.y +
                    r;

                vel.y *= -Config::BALL_BOUNCE;
            }
            // Nếu bóng đang rơi xuống thì không cần xử lý
        }

        // =========================
        // WALL DỌC - LEFT / RIGHT
        // =========================
        else
        {
            if (vel.x < 0.f)
            {
                // Đụng wall bên trái
                pos.x =
                    wallRect.position.x +
                    wallRect.size.x +
                    r;
            }
            else if (vel.x > 0.f)
            {
                // Đụng wall bên phải
                pos.x =
                    wallRect.position.x -
                    r;
            }

            vel.x *= -Config::BALL_BOUNCE;
        }

        ball.setPosition(pos);
        ball.setVelocity(vel);
}




// va chạm giữa thiên thạch và người chơi

void Collision::handlePlayerAsteroid(Player& player, Asteroid& asteroid)
{
    if (!asteroid.active || asteroid.spawnDelay > 0.f || !asteroid.sprite.has_value())
        return;

    sf::FloatRect astHitbox = asteroid.sprite->getGlobalBounds();
    sf::FloatRect playerHitbox = player.getBodyHitbox();

    if (!astHitbox.findIntersection(playerHitbox).has_value())
        return;

    asteroid.active = false;
    player.stun(Config::ASTEROID_STUN_DURATION);
    AudioManager::getInstance().playSound("stun");
}