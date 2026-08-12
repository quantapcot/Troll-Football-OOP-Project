#include "entities/Player.h"
#include "audio/AudioManager.h"
#include "core/AssetManager.h"
#include "core/GameConfig.h"
#include <algorithm>
#include <cstdint>
#include <iostream>


Player::Player(const ControlScheme &controls, const sf::Color &color)
    : controls(controls), playerColor(color) {

  facingRight = (playerColor == sf::Color::Red);

  // Chọn texture theo Player
  if (playerColor == sf::Color::Red) {
      sprite.emplace(AssetManager::get().getTexture("char_ronaldo"));
  }
  else {
      sprite.emplace(AssetManager::get().getTexture("char_messi"));
  }



  // =========================
  // SHOE SPRITE INITIALIZATION
  // =========================
  try {
    if (playerColor == sf::Color::Red) {
      shoeSprite.emplace(AssetManager::get().getTexture("shoePlayer1"));
      shoeSprite->setTextureRect(sf::IntRect({ 478, 875 }, { 128, 62 }));
      shoeSprite->setOrigin({ 20.f, 40.f });
    } else {
      shoeSprite.emplace(AssetManager::get().getTexture("shoePlayer2"));
      shoeSprite->setTextureRect(sf::IntRect({ 455, 865 }, { 132, 59 }));
      shoeSprite->setOrigin({ 20.f, 40.f });
    }
  } catch (...) {
    std::cout << "[Player] Warning: Could not load shoe texture!" << std::endl;
  }

  // Vị trí ban đầu
  position = {100.f, Config::GROUND_Y - Config::PLAYER_HALF_HEIGHT};

  sprite->setPosition(position);
}

void Player::stun(float durationSeconds) {
  // Tùy chỉnh duration dựa trên stunCooldown (Stun Recovery) của character.
  // Base stat = 5 -> hệ số 1.0. Stat 10 -> hệ số 0.75. Stat 1 -> hệ số 1.25.
  float durationModifier = 1.0f;
  if (m_characterStats.stunCooldown > 0) {
      durationModifier = 0.8f + (5 - m_characterStats.stunCooldown) * 0.05f; 
      // stat = 9 -> 0.8 + (-4)*0.05 = 0.6
      // Let's use simpler: 1.0f - (stat - 5) * 0.05f.
      // Stat 5 -> 1.0. Stat 10 -> 0.75. Stat 1 -> 1.2.
      durationModifier = 1.0f - (m_characterStats.stunCooldown - 5) * 0.05f;
  }
  stunEffect.start(durationSeconds * durationModifier);

  // Hiệu ứng đỏ ngay lập tức khi bị stun
  if (sprite) {
    sprite->setColor(sf::Color(255, 80, 80, 255));
  }

  // Ngăn player tiếp tục giữ vận tốc cũ
  velocity = {0.f, 0.f};
}

void Player::update(float deltaTime) {

  // =========================
  // TRẠNG THÁI CHOÁNG (1.0S)
  // =========================
  if (stunEffect.isActive()) {
    stunEffect.update(deltaTime);
    stunEffect.setPosition(position);

    // Flash đỏ/cam theo nhịp để tạo hiệu ứng giật
    if (sprite) {
      const float FLASH_INTERVAL = 0.1f;
      int flashTick = static_cast<int>(stunEffect.getRemainingTime() / FLASH_INTERVAL);
      if (flashTick % 2 == 0)
        sprite->setColor(sf::Color(255, 80, 80, 255)); // Đỏ
      else
        sprite->setColor(sf::Color(255, 180, 80, 255)); // Cam
    }

    // Trong thời gian stun:
    // không di chuyển, không kick, không jump, không dash
    velocity = {0.f, 0.f};

    if (sprite)
      sprite->setPosition(position);

    return;
  }

  // Hết stun: khôi phục màu bình thường
  if (sprite.has_value()) {
    sprite->setColor(sf::Color::White);
  }

  if (!aiControlled) {
    currentInput.left = sf::Keyboard::isKeyPressed(controls.left);

    currentInput.right = sf::Keyboard::isKeyPressed(controls.right);

    currentInput.jump = sf::Keyboard::isKeyPressed(controls.jump);

    currentInput.kick = sf::Keyboard::isKeyPressed(controls.kick);

    currentInput.dash = sf::Keyboard::isKeyPressed(controls.dash);
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

void Player::updateKick(float deltaTime) {
  if (kickCooldown > 0.f) {
    kickCooldown -= deltaTime;
  }

  if (kicking) {
    kickTimer -= deltaTime;

    if (kickTimer <= 0.f) {
      kicking = false;
      kickHit = false;
    }
  }
}

void Player::updateDash(float deltaTime) {
  // Giảm cooldown
  if (dashCooldown > 0.f) {
    dashCooldown -= deltaTime;
  }

  // Dash đang diễn ra
  if (dashing) {
    dashTimer -= deltaTime;

    if (dashTimer <= 0.f) {
      dashing = false;
    }
  }
}

void Player::updateTrail(float deltaTime) {
  // Giảm thời gian sống của trail
  for (auto &trail : trails) {
    trail.life -= deltaTime;
  }

  // Xóa trail đã hết thời gian
  trails.erase(std::remove_if(trails.begin(), trails.end(),
                              [](const Trail &t) { return t.life <= 0.f; }),
               trails.end());

  // Chỉ tạo trail khi đang dash
  if (!dashing) {
    trailSpawnTimer = 0.f;
    return;
  }

  trailSpawnTimer += deltaTime;

  if (trailSpawnTimer >= trailSpawnInterval) {
    trailSpawnTimer = 0.f;

    trails.push_back({position, Config::TRAIL_LIFETIME});
  }
}

void Player::handleMovementInput() {
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

void Player::handleKickInput() {
  if (currentInput.kick) {
    if (!kicking && kickCooldown <= 0.f) {
      kicking = true;
      kickHit = false;
      kickTimer = kickDuration;
      kickCooldown = kickCooldownTime;
    }
  }
}

void Player::handleDashInput() {
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

  if (currentInput.left) {
    dashDirection = -1;
  } else if (currentInput.right) {
    dashDirection = 1;
  } else {
    // Nếu không bấm hướng thì dash theo hướng nhìn
    dashDirection = facingRight ? 1 : -1;
  }
}

void Player::handleJumpInput() {
  if (currentInput.jump && onGround) {
    velocity.y = jumpForce;
    onGround = false;
    AudioManager::getInstance().playSound("jump");
  }
}

void Player::applyGravity(float deltaTime) {
  velocity.y += gravity * deltaTime;
}

void Player::move(float deltaTime) { position += velocity * deltaTime; }

void Player::handleGroundCollision() {
  const float groundY = Config::GROUND_Y - Config::PLAYER_HALF_HEIGHT;

  if (position.y >= groundY) {
    position.y = groundY;
    velocity.y = 0.f;
    onGround = true;
  } else {
    onGround = false;
  }
}

void Player::handleWallCollision() {
  if (position.x < Config::PLAYER_HALF_WIDTH) {
    position.x = Config::PLAYER_HALF_WIDTH;
  }

  if (position.x > Config::WINDOW_WIDTH - Config::PLAYER_HALF_WIDTH) {
    position.x = Config::WINDOW_WIDTH - Config::PLAYER_HALF_WIDTH;
  }
}

void Player::render(sf::RenderWindow &window) {
  // =========================
  // DASH TRAIL
  // =========================

  for (const auto &trail : trails) {
    sf::Sprite ghost(*sprite);

    ghost.setPosition(trail.position);

    auto alpha = static_cast<std::uint8_t>(255.f * (trail.life / Config::TRAIL_LIFETIME));

    sf::Color ghostColor = sprite->getColor();
    ghostColor.a = alpha;

    ghost.setColor(ghostColor);

    window.draw(ghost);
  }

  // =========================
  // PLAYER
  // =========================

  if (sprite) {
    window.draw(*sprite);
  }

  // =========================
  // STUN EFFECT
  // =========================
  stunEffect.render(window);

  // =========================
  // SHOE KICK ANIMATION
  // =========================
  if (kicking && shoeSprite.has_value()) {
    float progress = 1.0f - std::clamp(kickTimer / kickDuration, 0.0f, 1.0f);
    float swing = std::sin(progress * 3.14159265f); // 0.0 -> 1.0 -> 0.0

    // Scale giày tỉ lệ nhỏ vừa bằng đúng bàn chân nhân vật (~28px)
    float shoeScale = 0.22f;
    float scaleX = facingRight ? shoeScale : -shoeScale;
    shoeSprite->setScale({ scaleX, shoeScale });

    // Vị trí bắt đầu cực thấp ở sát dưới đất/bàn chân
    float baseX = facingRight ? (position.x + 12.f) : (position.x - 12.f);
    float baseY = position.y + 60.f; // Xuất phát sát dưới mặt cỏ

    // Quỹ đạo sút bổng: Bắt đầu sát đất rồi vung cao nảy lên không trung như ban đầu
    float forwardOffset = facingRight ? (45.f * swing) : (-45.f * swing);
    float upwardOffset = -50.f * swing; // Vung cao lên không trung như ban đầu

    shoeSprite->setPosition({ baseX + forwardOffset, baseY + upwardOffset });

    // Góc xoay sút vổng mạnh mẽ lên trên (-75 deg khi quay phải, +75 deg khi quay trái)
    float rotAngle = facingRight ? (-75.f * swing) : (75.f * swing);
    shoeSprite->setRotation(sf::degrees(rotAngle));

    window.draw(*shoeSprite);
  }
}

sf::FloatRect Player::getKickHitbox() const {
  if (!kicking)
    return sf::FloatRect();

  float x;

  // Đẩy Hitbox sút ra phía trước tương ứng với chiếc giày vung cao
  if (facingRight) {
    x = position.x + Config::PLAYER_HALF_WIDTH + 10.f;
  } else {
    x = position.x - Config::PLAYER_HALF_WIDTH - Config::WIDTH_KICK_HITBOX - 10.f;
  }

  return sf::FloatRect({x, position.y - Config::PLAYER_HALF_HEIGHT + 45.f},
                       {Config::WIDTH_KICK_HITBOX, Config::HEIGHT_KICK_HITBOX});
}

void Player::setSkin(const sf::Texture& texture, bool flipHorizontal, float scale) {
    sprite.emplace(texture);
    auto size = sprite->getTexture().getSize();
    sprite->setOrigin({ size.x / 2.f, size.y / 2.f });

    float scaleX = flipHorizontal ? -scale : scale;
    sprite->setScale({ scaleX, scale });
    sprite->setPosition(position);
}

void Player::setCharacterStats(const CharacterOption& option) {
    m_characterStats = option;

    // Stat 5 is average (multiplier 1.0). Each point above/below alters by 5%.
    float speedMultiplier = 0.8f + (option.speed - 5) * 0.05f;
    float jumpMultiplier = 0.8f + (option.jump - 5) * 0.05f;

    moveSpeed = Config::PLAYER_SPEED * speedMultiplier;
    jumpForce = Config::PLAYER_JUMP_FORCE * jumpMultiplier;
}

float Player::getKickForceX() const {
    float modifier = 1.0f;
    if (m_characterStats.kick > 0) {
        modifier = 0.8f + (m_characterStats.kick - 5) * 0.05f;
    }
    return Config::PLAYER_KICK_FORCE_X * modifier;
}

float Player::getKickForceY() const {
    float modifier = 1.0f;
    if (m_characterStats.kick > 0) {
        modifier = 0.8f + (m_characterStats.kick - 5) * 0.05f;
    }
    return Config::PLAYER_KICK_FORCE_Y * modifier;
}

sf::FloatRect Player::getBodyHitbox() const {
  return {{position.x - Config::WIDTH_BODY_HITBOX * 0.5f,
           position.y - Config::HEIGHT_BODY_HITBOX * 0.5f},
          {Config::WIDTH_BODY_HITBOX, Config::HEIGHT_BODY_HITBOX}};
}
