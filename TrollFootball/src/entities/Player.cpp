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
    sprite.emplace(AssetManager::get().getTexture("player1"));

  } else {
    sprite.emplace(AssetManager::get().getTexture("player2"));
  }

  // =========================
  // LOAD STUN EFFECT
  // =========================

  AssetManager &assets = AssetManager::get();

  stunTextures[0] = &assets.getTexture("stun1");

  stunTextures[1] = &assets.getTexture("stun2");

  stunTextures[2] = &assets.getTexture("stun3");

  stunTextures[3] = &assets.getTexture("stun4");

  stunTextures[4] = &assets.getTexture("stun5");

  // Lấy kích thước ảnh
  auto size = sprite->getTexture().getSize();
  std::cout << "Player texture size: " << size.x << " x " << size.y
            << std::endl;

  // Đặt tâm Sprite
  sprite->setOrigin({size.x / 2.f, size.y / 2.f});

  // Scale về đúng kích thước nhân vật trong game
  sprite->setScale({0.18f, 0.18f});

  // =========================
  // STUN SPRITE
  // =========================

  if (!stunTextures.empty()) {
    stunSprite.emplace(*stunTextures[0]);

    auto stunSize = stunTextures[0]->getSize();

    stunSprite->setOrigin({stunSize.x / 2.f, stunSize.y / 2.f});

    // Scale đủ lớn để hiện rõ trên đầu player
    stunSprite->setScale({0.5f, 0.5f});

    // Thực tế: player texture 1080x1920 scale 0.18f
    // đỉnh đầu sprite = position.y - (1920/2)*0.18 = position.y - 172.8px
    // Đặt stunSprite ngay trên đỉnh đầu sprite player
    stunSprite->setPosition({position.x, position.y - 85.f});
  }

  // Vị trí ban đầu
  position = {100.f, Config::GROUND_Y - Config::PLAYER_HALF_HEIGHT};

  sprite->setPosition(position);
}

void Player::stun(float durationSeconds) {
  stunTimer = durationSeconds;

  // Reset animation về frame đầu
  stunFrameTimer = 0.f;
  stunFrame = 0;

  std::cout << "[STUN] stun() called. stunSprite has_value="
            << stunSprite.has_value() << " stunTextures[0]=" << stunTextures[0]
            << " stunTimer=" << stunTimer << std::endl;

  if (stunSprite && stunTextures[0]) {
    stunSprite->setTexture(*stunTextures[0], true); // resetRect=true
    // Cập nhật lại origin sau khi đổi texture
    auto sz = stunTextures[0]->getSize();
    stunSprite->setOrigin({sz.x / 2.f, sz.y / 2.f});
    stunSprite->setPosition({position.x, position.y - 85.f});
    std::cout << "[STUN] stunSprite texture set, size=" << sz.x << "x" << sz.y
              << " pos=" << position.x << "," << (position.y - 85.f)
              << std::endl;
  } else {
    std::cout << "[STUN] WARNING: stunSprite or stunTextures[0] is null!"
              << std::endl;
  }

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
  if (stunTimer > 0.f) {
    updateStun(deltaTime);

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

    trails.push_back({position, 0.15f});
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

    // Trail càng cũ càng trong suốt
    auto alpha = static_cast<std::uint8_t>(255.f * (trail.life / 0.15f));

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

  if (stunTimer > 0.f && stunSprite) {
    // position.y - 190f: ngay trên đỉnh đầu sprite player (1080x1920, scale
    // 0.18f)
    stunSprite->setPosition({position.x, position.y - 85.f});

    window.draw(*stunSprite);
  }

#ifdef _DEBUG

  auto body = getBodyHitbox();

  sf::RectangleShape rect;

  rect.setPosition(body.position);

  rect.setSize(body.size);

  rect.setFillColor(sf::Color(0, 255, 0, 80));

  window.draw(rect);

#endif

#ifdef _DEBUG

  if (kicking) {
    sf::RectangleShape hitbox;

    auto rect = getKickHitbox();

    hitbox.setPosition(rect.position);
    hitbox.setSize(rect.size);

    hitbox.setFillColor(sf::Color(255, 255, 0, 80));

    window.draw(hitbox);
  }

#endif
}

sf::FloatRect Player::getKickHitbox() const {
  if (!kicking)
    return sf::FloatRect();

  float x;

  if (facingRight) {
    x = position.x + Config::PLAYER_HALF_WIDTH;
  } else {
    x = position.x - Config::PLAYER_HALF_WIDTH - Config::WIDTH_KICK_HITBOX;
  }

  return sf::FloatRect({x, position.y - Config::PLAYER_HALF_HEIGHT + 45.f},
                       {Config::WIDTH_KICK_HITBOX, Config::HEIGHT_KICK_HITBOX});
}

void Player::setSkin(const sf::Texture &texture, bool flipHorizontal,
                     float scale) {
  sprite.emplace(texture);

  auto size = sprite->getTexture().getSize();

  sprite->setOrigin({size.x / 2.f, size.y / 2.f});

  // flipHorizontal = true -> dao scale.x am de lat nguoc anh qua truc doc
  float scaleX = flipHorizontal ? -scale : scale;

  sprite->setScale({scaleX, scale});
  sprite->setPosition(position);
}

sf::FloatRect Player::getBodyHitbox() const {
  return {{position.x - Config::WIDTH_BODY_HITBOX * 0.5f,
           position.y - Config::HEIGHT_BODY_HITBOX * 0.5f},
          {Config::WIDTH_BODY_HITBOX, Config::HEIGHT_BODY_HITBOX}};
}

void Player::updateStun(float deltaTime) {
  if (stunTimer <= 0.f)
    return;

  stunTimer -= deltaTime;

  // Flash đỏ/cam theo nhịp để tạo hiệu ứng giật
  if (sprite) {
    const float FLASH_INTERVAL = 0.1f;
    int flashTick = static_cast<int>(stunTimer / FLASH_INTERVAL);
    if (flashTick % 2 == 0)
      sprite->setColor(sf::Color(255, 80, 80, 255)); // Đỏ
    else
      sprite->setColor(sf::Color(255, 180, 80, 255)); // Cam
  }

  stunFrameTimer += deltaTime;

  if (stunFrameTimer >= STUN_FRAME_DURATION) {
    stunFrameTimer = 0.f;

    stunFrame++;

    if (stunFrame >= STUN_FRAME_COUNT)
      stunFrame = 0;

    if (stunSprite && stunTextures[stunFrame]) {
      stunSprite->setTexture(*stunTextures[stunFrame], true);
      // Cập nhật origin sau khi đổi texture
      auto sz = stunTextures[stunFrame]->getSize();
      stunSprite->setOrigin({sz.x / 2.f, sz.y / 2.f});
    }
  }

  // Vị trí stunSprite: trên đỉnh đầu sprite player (cập nhật mỗi frame)
  if (stunSprite) {
    stunSprite->setPosition({position.x, position.y - 85.f});
  }

  // Khi vừa hết stun: reset màu
  if (stunTimer <= 0.f && sprite) {
    sprite->setColor(sf::Color::White);
  }
}