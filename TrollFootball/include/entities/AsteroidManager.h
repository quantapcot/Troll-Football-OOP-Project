#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <optional>
#include <cstdint>

struct Asteroid
{
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f startPos;
    float totalDistance{ 1.0f };
    float spawnDelay{ 0.f };
    bool active{ false };
    std::optional<sf::Sprite> sprite;
};

class Player;

class AsteroidManager
{
public:
    AsteroidManager();

    void init(const sf::Texture& texture);
    void startMatch(float matchDurationSeconds = 90.f);
    void reset();

    void update(float deltaTime, float matchElapsedTime);
    void checkPlayerCollisions(Player& player1, Player& player2);
    void render(sf::RenderWindow& window);

private:
    void triggerWave();

    sf::Texture m_texture;
    bool m_hasTexture{ false };
    int m_frameWidth{ 32 };
    int m_frameHeight{ 32 };

    float m_matchDuration{ 90.f };
    std::vector<float> m_waveTimes;
    std::vector<bool> m_waveTriggered;

    std::vector<Asteroid> m_asteroids;
    std::mt19937 m_rng;
};
