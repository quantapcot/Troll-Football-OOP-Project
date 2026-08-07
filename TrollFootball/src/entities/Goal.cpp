#include "entities/Goal.h"
#include "core/GameConfig.h"
#include "entities/Ball.h"

Goal::Goal(float x,
    bool isRightSide,
    const sf::Texture& texture)
{
    //=========================
    // GOAL TRIGGER
    //=========================

    goalTrigger.setSize({
        Config::GOAL_WIDTH,
        Config::GOAL_HEIGHT
        });
#ifdef _DEBUG
    goalTrigger.setFillColor(
        sf::Color(0, 255, 0, 60));
#else
    goalTrigger.setFillColor(
        sf::Color::Transparent);
#endif

    //=========================
    // SPRITE
    //=========================

    sprite.emplace(texture);

    auto texSize = texture.getSize();

    float scale =
        Config::GOAL_VISUAL_HEIGHT /
        static_cast<float>(texSize.y);

    sprite->setScale({
        scale,
        scale
        });

    if (!isRightSide)
    {
        sprite->setOrigin({
            204.f,                    // đúng bằng mép cột trước trong PNG
            static_cast<float>(texSize.y)
            });

        sprite->setPosition({
            x,
            Config::GROUND_Y + 53.f
            });
    }
    else
    {
        sprite->setOrigin({
            906.f,                    // đúng bằng mép cột trước trong PNG
            static_cast<float>(texSize.y)
            });

        sprite->setPosition({
            x,
            Config::GROUND_Y + 53.f
            });
    }

    sprite->setPosition({
        x,
        Config::GROUND_Y + 53.f
        });

    //-------------------------------------------------
    // Từ đây trở đi collider sẽ căn theo Sprite
    //-------------------------------------------------

    auto bounds = sprite->getGlobalBounds();

    // =========================
    // CẬP NHẬT VỊ TRÍ GOAL TRIGGER
    // =========================
    if (!isRightSide)
    {
        // Bên trái giữ nguyên gốc x (vẽ từ x sang phải)
        goalTrigger.setPosition({
            x,
            Config::GROUND_Y - Config::GOAL_HEIGHT
            });
    }
    else
    {
        // Bên phải: lùi sang trái 1 khoảng bằng GOAL_WIDTH rồi vẽ sang phải (vào trong lưới)
        goalTrigger.setPosition({
            x - Config::GOAL_WIDTH,
            Config::GROUND_Y - Config::GOAL_HEIGHT
            });
    }

    //=========================
    // CROSSBAR
    //=========================

    // THÊM BIẾN NÀY ĐỂ TỰ DO KÉO DÀI THANH ĐỎ
    // Tăng số 140.f lên nếu muốn thanh đỏ dài ra thêm, giảm đi nếu nó bị lố ra ngoài
    float crossbarWidth = 100.f;

    crossbar.setSize({ crossbarWidth, 8.f });

#ifdef _DEBUG
    crossbar.setFillColor(
        sf::Color(255, 0, 0, 120));
#else
    crossbar.setFillColor(
        sf::Color::Transparent);
#endif

    if (isRightSide)
    {
        // Khung thành phải (Messi): Mỏ neo lùi ra tận mép ngoài xà ngang, rồi vẽ dài vào tới đuôi lưới (x)
        crossbar.setPosition({
            x - crossbarWidth,
            Config::GROUND_Y - Config::GOAL_HEIGHT - 65.f
            });
    }
    else
    {
        // Khung thành trái (CR7): Mỏ neo ở đuôi lưới (x), vẽ dài ra tận mép ngoài xà ngang
        crossbar.setPosition({
            x,
            Config::GROUND_Y - Config::GOAL_HEIGHT - 65.f
            });
    }

    //=========================
    // BACK WALL
    //=========================

    backWall.setSize({
        8.f,
        Config::GOAL_HEIGHT
        });
#ifdef _DEBUG
    backWall.setFillColor(
        sf::Color(0, 0, 255, 120));
#else
    backWall.setFillColor(
        sf::Color::Transparent);
#endif

    if (isRightSide)
    {
        backWall.setPosition({
            x - 8.f,
            Config::GROUND_Y -
            Config::GOAL_HEIGHT
            });
    }
    else
    {
        // Đồng thời sửa luôn lưới xanh dương bên CR7 cho nó chặn đúng vạch lưới
        backWall.setPosition({
            x,
            Config::GROUND_Y -
            Config::GOAL_HEIGHT
            });
    }
}

void Goal::update(float deltaTime)
{
    // Chưa cần xử lý
}

void Goal::render(sf::RenderWindow& window)
{
    goalTrigger.setFillColor(sf::Color(0, 255, 0, 80));
    crossbar.setFillColor(sf::Color(255, 0, 0, 120));
    backWall.setFillColor(sf::Color(0, 0, 255, 120));

    window.draw(goalTrigger);
    window.draw(crossbar);
    window.draw(backWall);

    if (sprite)
        window.draw(*sprite);
}

bool Goal::contains(const Ball& ball) const
{
    return goalTrigger
        .getGlobalBounds()
        .contains(ball.getPosition());
}

void Goal::handleCollision(Ball& ball)
{

}