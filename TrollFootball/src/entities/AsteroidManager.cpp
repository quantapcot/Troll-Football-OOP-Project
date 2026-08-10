#include "entities/AsteroidManager.h"
#include "entities/Player.h"
#include "core/GameConfig.h"
#include <audio/AudioManager.h>
#include <algorithm>
#include <cmath>
#include <iostream>

AsteroidManager::AsteroidManager()
    : m_rng(std::random_device{}())
{
}

void AsteroidManager::init(const sf::Texture& texture)
{
    m_texture = texture;
    m_hasTexture = true;

    std::cout << "[AsteroidManager] Texture loaded successfully: "
              << m_texture.getSize().x << "x" << m_texture.getSize().y << std::endl;
}

void AsteroidManager::reset()
{
    m_asteroids.clear();
    m_waveTriggered.assign(3, false);
}

void AsteroidManager::startMatch(float matchDurationSeconds)
{
    reset();
    m_matchDuration = matchDurationSeconds;

    // Chia 90s thi đấu thành 3 đợt rơi thiên thạch ngẫu nhiên
    std::uniform_real_distribution<float> dist1(m_matchDuration * 0.12f, m_matchDuration * 0.30f);
    std::uniform_real_distribution<float> dist2(m_matchDuration * 0.40f, m_matchDuration * 0.60f);
    std::uniform_real_distribution<float> dist3(m_matchDuration * 0.70f, m_matchDuration * 0.88f);

    m_waveTimes = { dist1(m_rng), dist2(m_rng), dist3(m_rng) };
    m_waveTriggered = { false, false, false };

    std::cout << "[AsteroidManager] Scheduled 3 waves at: "
              << m_waveTimes[0] << "s, " << m_waveTimes[1] << "s, " << m_waveTimes[2] << "s" << std::endl;
}

void AsteroidManager::triggerWave()
{
    if (!m_hasTexture)
        return;

	AudioManager::getInstance().playSound("asteroid");

    std::uniform_int_distribution<int> countDist(Config::ASTEROID_MIN_COUNT, Config::ASTEROID_MAX_COUNT);
    int count = countDist(m_rng);

    // Kích thước thiên thạch
    const float targetScale = Config::ASTEROID_SCALE;

    std::uniform_real_distribution<float> startXDist(-350.f, Config::WINDOW_WIDTH * 0.55f);
    std::uniform_real_distribution<float> startYDist(-300.f, -80.f);
    // Tốc độ rơi
    std::uniform_real_distribution<float> speedDist(Config::ASTEROID_MIN_SPEED, Config::ASTEROID_MAX_SPEED);
    std::uniform_real_distribution<float> delayDist(0.0f, Config::ASTEROID_MAX_SPAWN_DELAY);

    for (int i = 0; i < count; ++i)
    {
        float speed = speedDist(m_rng);

        Asteroid ast;
        ast.startPos = { startXDist(m_rng), startYDist(m_rng) };
        ast.position = ast.startPos;

        // Quỹ đạo bay chéo đúng góc 45 độ (vx = vy = speed)
        ast.velocity = { speed, speed };
        ast.spawnDelay = delayDist(m_rng);
        ast.active = true;

        // Tọa độ thoát khỏi màn hình
        float exitY = Config::WINDOW_HEIGHT + 200.f;
        float exitX = ast.startPos.x + (exitY - ast.startPos.y);
        ast.totalDistance = std::hypot(exitX - ast.startPos.x, exitY - ast.startPos.y);

        ast.sprite.emplace(m_texture);

        // Frame 1 ban đầu (Tọa độ chính xác: X=0, Y=0, W=24, H=32)
        ast.sprite->setTextureRect(sf::IntRect({ 0, 0 }, { 24, 32 }));
        ast.sprite->setOrigin({ 12.f, 16.f });
        ast.sprite->setScale({ targetScale, targetScale });

        // Xoay hình thiên thạch theo đúng đường bay 45 độ
        ast.sprite->setRotation(sf::degrees(-45.f));

        ast.sprite->setPosition(ast.position);

        m_asteroids.push_back(ast);
    }

    
}

void AsteroidManager::update(float deltaTime, float matchElapsedTime)
{
    // Kiểm tra kích hoạt 3 đợt rơi theo thời gian trận đấu
    for (size_t i = 0; i < m_waveTimes.size(); ++i)
    {
        if (!m_waveTriggered[i] && matchElapsedTime >= m_waveTimes[i])
        {
            m_waveTriggered[i] = true;
            triggerWave();
        }
    }

    // Cập nhật đường bay 45 độ và chuyển đổi 3 dạng frame chính xác
    for (auto& ast : m_asteroids)
    {
        if (!ast.active || !ast.sprite.has_value())
            continue;

        // Nếu đang trong thời gian chờ trễ (delay) ban đầu
        if (ast.spawnDelay > 0.f)
        {
            ast.spawnDelay -= deltaTime;
            continue;
        }

        // Bay theo góc 45 độ chéo xuống dưới bên phải
        ast.position += ast.velocity * deltaTime;
        ast.sprite->setPosition(ast.position);

        // Tỉ lệ tiến trình bay (0.0 -> 1.0)
        float dx = ast.position.x - ast.startPos.x;
        float dy = ast.position.y - ast.startPos.y;
        float traveled = std::hypot(dx, dy);
        float progress = (ast.totalDistance > 0.f) ? (traveled / ast.totalDistance) : 1.f;
        progress = std::clamp(progress, 0.0f, 1.0f);

        // =========================================================================
        // TỌA ĐỘ CẮT CHÍNH XÁC TỪNG THIÊN THẠCH:
        // - Frame 1 (Dạng 1): X = 0..24  (24px) -> sf::IntRect({ 0,  0 }, { 24, 32 })
        // - Frame 2 (Dạng 2): X = 24..50 (26px) -> sf::IntRect({ 24, 0 }, { 26, 32 })
        // - Frame 3 (Dạng 3): X = 50..76 (26px) -> sf::IntRect({ 50, 0 }, { 26, 32 })
        // =========================================================================
        if (progress < 0.33f)
        {
            ast.sprite->setTextureRect(sf::IntRect({ 0, 0 }, { 24, 32 }));
            ast.sprite->setOrigin({ 12.f, 16.f });
        }
        else if (progress < 0.66f)
        {
            ast.sprite->setTextureRect(sf::IntRect({ 24, 0 }, { 26, 32 }));
            ast.sprite->setOrigin({ 13.f, 16.f });
        }
        else
        {
            ast.sprite->setTextureRect(sf::IntRect({ 50, 0 }, { 26, 32 }));
            ast.sprite->setOrigin({ 13.f, 16.f });
        }

        // Khi thiên thạch bay ra khỏi màn hình
        if (ast.position.x > Config::WINDOW_WIDTH + 200.f || ast.position.y > Config::WINDOW_HEIGHT + 200.f)
        {
            ast.active = false;
        }
    }
}

void AsteroidManager::checkPlayerCollisions(Player& player1, Player& player2)
{
    if (!m_hasTexture)
        return;

    sf::FloatRect p1Hitbox = player1.getBodyHitbox();
    sf::FloatRect p2Hitbox = player2.getBodyHitbox();

    for (auto& ast : m_asteroids)
    {
        if (!ast.active || ast.spawnDelay > 0.f || !ast.sprite.has_value())
            continue;

        sf::FloatRect astHitbox = ast.sprite->getGlobalBounds();

        // Va chạm Player 1: Thiên thạch biến mất, Player 1 choáng 0.5s
        if (astHitbox.findIntersection(p1Hitbox).has_value())
        {
            ast.active = false;
            player1.stun(Config::ASTEROID_STUN_DURATION);
			AudioManager::getInstance().playSound("stun");
        }
        // Va chạm Player 2: Thiên thạch biến mất, Player 2 choáng 0.5s
        else if (astHitbox.findIntersection(p2Hitbox).has_value())
        {
            ast.active = false;
            player2.stun(Config::ASTEROID_STUN_DURATION);
            AudioManager::getInstance().playSound("stun");
        }
    }
}

void AsteroidManager::render(sf::RenderWindow& window)
{
    if (!m_hasTexture)
        return;

    for (const auto& ast : m_asteroids)
    {
        if (ast.active && ast.spawnDelay <= 0.f && ast.sprite.has_value())
        {
            window.draw(*ast.sprite);
        }
    }
}
