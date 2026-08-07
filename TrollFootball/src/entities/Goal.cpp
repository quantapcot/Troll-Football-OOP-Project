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
            204.f,
            static_cast<float>(texSize.y)
            });
    }
    else
    {
        sprite->setOrigin({
            906.f,
            static_cast<float>(texSize.y)
            });
    }

    
    spritePosition =
    {
        x,
        Config::GROUND_Y + 53.f
    };

    sprite->setPosition(spritePosition);

    //-------------------------------------------------
    // Từ đây trở đi collider sẽ căn theo Sprite
    //-------------------------------------------------

    

    // =========================
    // CẬP NHẬT VỊ TRÍ GOAL TRIGGER
    // =========================
    if (!isRightSide)
    {
        triggerPosition =
        {
            x + 40.f,
            Config::GROUND_Y - Config::GOAL_HEIGHT
        };
    }
    else
    {
        triggerPosition =
        {
            x - Config::GOAL_WIDTH - 40,
            Config::GROUND_Y - Config::GOAL_HEIGHT
        };
    }

    goalTrigger.setPosition(triggerPosition);

    //=========================
    // CROSSBAR
    //=========================

    // THÊM BIẾN NÀY ĐỂ TỰ DO KÉO DÀI THANH ĐỎ
    // Tăng số 140.f lên nếu muốn thanh đỏ dài ra thêm, giảm đi nếu nó bị lố ra ngoài
    float crossbarWidth = 100.f;

    crossbar.setSize({ crossbarWidth, 8.f });



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