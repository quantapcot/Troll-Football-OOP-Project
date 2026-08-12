#include "entities/Stun.h"
#include "core/AssetManager.h"
#include <iostream>

Stun::Stun() : timer(0.f), frame(0), frameTimer(0.f)
{
    auto& assets = AssetManager::get();
    textures[0] = &assets.getTexture("stun1");
    textures[1] = &assets.getTexture("stun2");
    textures[2] = &assets.getTexture("stun3");
    textures[3] = &assets.getTexture("stun4");
    textures[4] = &assets.getTexture("stun5");

    if (textures[0]) {
        sprite.emplace(*textures[0]);
        auto sz = textures[0]->getSize();
        sprite->setOrigin({sz.x / 2.f, sz.y / 2.f});
        sprite->setScale({Config::STUN_SPRITE_SCALE, Config::STUN_SPRITE_SCALE});
    }
}

void Stun::start(float durationSeconds)
{
    timer = durationSeconds;
    frame = 0;
    frameTimer = 0.f;

    if (sprite && textures[0]) {
        sprite->setTexture(*textures[0], true);
        auto sz = textures[0]->getSize();
        sprite->setOrigin({sz.x / 2.f, sz.y / 2.f});
    }
}

void Stun::update(float deltaTime)
{
    if (timer <= 0.f) return;

    timer -= deltaTime;
    if (timer < 0.f) {
        timer = 0.f;
    }

    frameTimer += deltaTime;
    if (frameTimer >= STUN_FRAME_DURATION) {
        frameTimer = 0.f;
        frame++;
        if (frame >= STUN_FRAME_COUNT) {
            frame = 0;
        }
        if (sprite && textures[frame]) {
            sprite->setTexture(*textures[frame], true);
        }
    }
}

void Stun::setPosition(const sf::Vector2f& position)
{
    if (sprite) {
        sprite->setPosition({position.x, position.y + Config::STUN_HEAD_OFFSET_Y});
    }
}

void Stun::render(sf::RenderWindow& window) const
{
    if (isActive() && sprite) {
        window.draw(*sprite);
    }
}

bool Stun::isActive() const
{
    return timer > 0.f;
}

float Stun::getRemainingTime() const
{
    return timer;
}
