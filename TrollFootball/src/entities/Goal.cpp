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
        scale + 1/50.f
        });

    if (!isRightSide)
    {
        sprite->setOrigin({
            213.f,
            static_cast<float>(texSize.y)
            });
    }
    else
    {
        sprite->setOrigin({
            925.f,
            static_cast<float>(texSize.y)
            });
    }

    
    spritePosition =
    {
        x,
        Config::GROUND_Y + 62.f
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

    crossbar.setSize({ crossbarWidth, 14.f });



    if (isRightSide)
    {
        // Khung thành phải (Messi): Mỏ neo lùi ra tận mép ngoài xà ngang, rồi vẽ dài vào tới đuôi lưới (x)
        crossbar.setPosition({
            x - crossbarWidth,
            Config::GROUND_Y - Config::GOAL_HEIGHT - 25.f
            });
    }
    else
    {
        // Khung thành trái (CR7): Mỏ neo ở đuôi lưới (x), vẽ dài ra tận mép ngoài xà ngang
        crossbar.setPosition({
            x,
            Config::GROUND_Y - Config::GOAL_HEIGHT - 25.f
            });
    }
}

// =========================
// UPDATE
// =========================

void Goal::update(float deltaTime)
{
    // Goal hiện tại không cần update
}

// =========================
// RENDER
// =========================



void Goal::render(sf::RenderWindow & window)
{
    // =========================
    // GOAL ASSET
    // =========================
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
    sf::Vector2f ballPos = ball.getPosition();
    float radius = ball.getRadius();

    sf::FloatRect ballRect(
        {
            ballPos.x - radius,
            ballPos.y - radius
        },
        {
            radius * 2.f,
            radius * 2.f
        });

        auto crossbarBounds = crossbar.getGlobalBounds();

        // Không chạm xà
        if (!crossbarBounds
            .findIntersection(ballRect)
            .has_value())
        {
            return;
        }

        sf::Vector2f velocity = ball.getVelocity();

        // Bóng đang rơi từ trên xuống
        if (velocity.y > 0.f)
        {
            // Đặt bóng lên phía trên xà
            ball.setPosition({
                ballPos.x,
                crossbarBounds.position.y - radius
                });

            // Bật ngược lên
            velocity.y =
                -std::abs(velocity.y) * Config::BALL_BOUNCE;

            ball.setVelocity(velocity);
        }

        // Bóng đang bay từ dưới lên
        else if (velocity.y < 0.f)
        {
            // Đặt bóng xuống dưới xà
            ball.setPosition({
                ballPos.x,
                crossbarBounds.position.y
                    + crossbarBounds.size.y
                    + radius
                });

            // Bật ngược xuống
            velocity.y =
                std::abs(velocity.y) * Config::BALL_BOUNCE;

            ball.setVelocity(velocity);
        }
}