#include "entities/Goal.h"
#include "core/GameConfig.h"
#include "entities/Ball.h"

Goal::Goal(float x, bool isRightSide, const sf::Texture& texture)
{
    // Hitbox: giữ nguyên kích thước/vị trí như cũ, dùng để check ghi bàn
    shape.setSize({
        Config::GOAL_WIDTH,
        Config::GOAL_HEIGHT
        });
    shape.setFillColor(sf::Color::Transparent);
    shape.setPosition({
        x,
        Config::GROUND_Y - Config::GOAL_HEIGHT
        });

    // Sprite: scale theo chiều cao riêng, giữ đúng tỉ lệ ảnh gốc (không méo)
    sprite.emplace(texture);

    auto texSize = texture.getSize();
    float scale = Config::GOAL_VISUAL_HEIGHT / static_cast<float>(texSize.y);
    sprite->setScale({ scale, scale });

    float visualWidth = static_cast<float>(texSize.x) * scale;

    if (isRightSide)
    {
        // Neo mép PHẢI của ảnh trùng mép phải cửa sổ, đáy trùng mặt đất
        sprite->setPosition({
            x + Config::GOAL_WIDTH - visualWidth,
            Config::GROUND_Y - Config::GOAL_VISUAL_HEIGHT
            });
    }
    else
    {
        // Neo mép TRÁI của ảnh trùng mép trái cửa sổ (x = 0), đáy trùng mặt đất
        sprite->setPosition({
            x,
            Config::GROUND_Y - Config::GOAL_VISUAL_HEIGHT
            });
    }
}

void Goal::update(float deltaTime)
{
    // Chưa cần xử lý
}

void Goal::render(sf::RenderWindow& window)
{
    if (sprite)
        window.draw(*sprite);

    // Debug hitbox nếu cần:
    // window.draw(shape);
}

bool Goal::contains(const Ball& ball) const
{
    return shape.getGlobalBounds().contains(ball.getPosition());
}