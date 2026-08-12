#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <optional>
#include "core/GameConfig.h"

class Stun
{
public:
    Stun();

    void start(float durationSeconds);
    void update(float deltaTime);
    void setPosition(const sf::Vector2f& position);
    void render(sf::RenderWindow& window) const;

    bool isActive() const;
    float getRemainingTime() const;

private:
    float timer;
    
    static constexpr int STUN_FRAME_COUNT = Config::STUN_FRAME_COUNT;
    static constexpr float STUN_FRAME_DURATION = Config::STUN_FRAME_DURATION;

    std::array<const sf::Texture*, STUN_FRAME_COUNT> textures{};
    std::optional<sf::Sprite> sprite;

    int frame;
    float frameTimer;
};
