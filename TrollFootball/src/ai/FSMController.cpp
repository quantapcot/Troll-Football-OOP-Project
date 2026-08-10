#include "ai/FSMController.h"

#include <cmath>
#include <algorithm>

// FSMController uses constants defined in Config namespace (include/core/GameConfig.h)
// - Config::AI_KICK_DISTANCE
// - Config::AI_STOP_DISTANCE
// - Config::AI_JUMP_HEIGHT
// - Config::AI_JUMP_DISTANCE
// - Config::AI_DEFEND_X
// - Config::AI_DASH_DISTANCE
// - Config::AI_PREDICT_TIME
// - Config::AI_STUCK_DISTANCE


FSMController::FSMController(
    Player& player,
    Ball& ball)
    : m_player(player),
    m_ball(ball)
{
}


void FSMController::update(float deltaTime)
{
    m_input = {};

    float ballX =
        m_ball.getPosition().x;

    // =====================================================
    // DEFEND
    // =====================================================

    // Bóng ở phần sân của Bot -> ưu tiên phòng thủ.
    if (ballX > Config::WINDOW_WIDTH * 0.70f)
    {
        m_state = BotState::Defend;
    }
    else if (m_state == BotState::Defend)
    {
        m_state = BotState::ChaseBall;
    }

    // =====================================================
    // FSM
    // =====================================================

    switch (m_state)
    {
    case BotState::Idle:
        updateIdle();
        break;

    case BotState::ChaseBall:
        updateChaseBall();
        break;

    case BotState::KickBall:
        updateKickBall();
        break;

    case BotState::Recover:
        updateRecover(deltaTime);
        break;

    case BotState::Defend:
        updateDefend();
        break;
    }
}


InputCommand FSMController::getInput() const
{
    return m_input;
}


// =========================================================
// IDLE
// =========================================================

void FSMController::updateIdle()
{
    // Bot không nên đứng yên quá lâu.
    // Luôn chuyển sang đuổi bóng.
    m_state = BotState::ChaseBall;
}


// =========================================================
// CHASE BALL
// =========================================================

void FSMController::updateChaseBall()
{
    float playerX =
        m_player.getPosition().x;

    float playerY =
        m_player.getPosition().y;

    sf::Vector2f ballPos =
        m_ball.getPosition();

    sf::Vector2f ballVel =
        m_ball.getVelocity();

    // =====================================================
    // DỰ ĐOÁN VỊ TRÍ BÓNG
    // =====================================================

    float ballX =
        ballPos.x +
        ballVel.x * Config::AI_PREDICT_TIME;

    float ballY =
        ballPos.y +
        ballVel.y * 0.10f;

    // =====================================================
    // TÍNH KHOẢNG CÁCH
    // =====================================================

    float dxBall =
        ballPos.x - playerX;

    float dyBall =
        ballPos.y - playerY;

    float distance =
        std::sqrt(
            dxBall * dxBall +
            dyBall * dyBall
        );

    // =====================================================
    // XÁC ĐỊNH VỊ TRÍ MUỐN ĐỨNG
    // =====================================================

    float targetX;

    if (m_player.isFacingRight())
    {
        // Bot đá về bên phải.
        // Muốn đứng bên trái bóng một chút.
        targetX =
            ballX - 38.f;
    }
    else
    {
        // Bot đá về bên trái.
        targetX =
            ballX + 38.f;
    }

    float dx =
        targetX - playerX;

    float dy =
        ballY - playerY;

    // =====================================================
    // BÓNG SÁT NGƯỜI
    // =====================================================
    //
    // Đây là phần sửa lỗi bot đứng đơ.
    //
    // Không được cứ thấy distance <= Config::AI_KICK_DISTANCE
    // là chuyển sang KickBall.
    //
    // Phải kiểm tra bóng có nằm đúng hướng để đá không.
    //

    if (distance <= Config::AI_STUCK_DISTANCE)
    {
        bool ballInFront;

        if (m_player.isFacingRight())
        {
            ballInFront =
                ballPos.x >= playerX - 5.f;
        }
        else
        {
            ballInFront =
                ballPos.x <= playerX + 5.f;
        }

        // Bóng ở đúng phía trước và khá thấp
        // -> chuyển sang KickBall.
        if (ballInFront &&
            std::abs(dy) < 65.f)
        {
            m_state = BotState::KickBall;
            return;
        }

        // -------------------------------------------------
        // Bóng sát nhưng ở phía sau
        // -------------------------------------------------
        //
        // Không được đứng yên.
        // Cho bot di chuyển về phía bóng.
        //

        if (m_player.isFacingRight())
        {
            if (ballPos.x < playerX)
            {
                m_input.left = true;
            }
            else
            {
                m_input.right = true;
            }
        }
        else
        {
            if (ballPos.x > playerX)
            {
                m_input.right = true;
            }
            else
            {
                m_input.left = true;
            }
        }

        // Nếu bóng ở trên đầu thì nhảy.
        if (dy < -Config::AI_JUMP_HEIGHT / 2.f)
        {
            m_input.jump = true;
        }

        return;
    }

    // =====================================================
    // ĐỦ GẦN ĐỂ ĐÁ
    // =====================================================

    if (distance <= Config::AI_KICK_DISTANCE &&
        std::abs(dy) < 65.f)
    {
        m_state = BotState::KickBall;
        return;
    }

    // =====================================================
    // DI CHUYỂN
    // =====================================================

    if (dx > Config::AI_STOP_DISTANCE)
    {
        m_input.right = true;
    }
    else if (dx < -Config::AI_STOP_DISTANCE)
    {
        m_input.left = true;
    }

    // =====================================================
    // JUMP
    // =====================================================

    // Bóng ở trên đầu và gần theo phương X.
    if (std::abs(dxBall) < Config::AI_JUMP_DISTANCE &&
        dy < -Config::AI_JUMP_HEIGHT)
    {
        m_input.jump = true;
    }

    // =====================================================
    // DASH
    // =====================================================
    // Dash khi:
    //
    // 1. Đang cách mục tiêu khá xa
    // 2. Hoặc bóng đang ở xa và đang chạy nhanh
    //
    // Player tự xử lý dash cooldown.
    //

    bool farFromTarget =
        std::abs(dx) > Config::AI_DASH_DISTANCE;

    bool ballMovingFast =
        std::abs(ballVel.x) > 80.f ||
        std::abs(ballVel.y) > 100.f;

    bool ballFar =
        distance > 130.f;

    if (farFromTarget &&
        ballFar)
    {
        m_input.dash = true;
    }
    else if (ballMovingFast &&
        ballFar &&
        std::abs(dx) > 100.f)
    {
        m_input.dash = true;
    }
}


// =========================================================
// KICK BALL
// =========================================================

void FSMController::updateKickBall()
{
    float playerX =
        m_player.getPosition().x;

    float playerY =
        m_player.getPosition().y;

    float ballX =
        m_ball.getPosition().x;

    float ballY =
        m_ball.getPosition().y;

    float dx =
        ballX - playerX;

    float dy =
        ballY - playerY;

    // =====================================================
    // BÓNG QUÁ XA
    // =====================================================
    //
    // Có thể Player đã bỏ lỡ bóng trong lúc chuyển state.
    //

    if (std::abs(dx) > 70.f ||
        std::abs(dy) > 80.f)
    {
        m_state = BotState::ChaseBall;
        return;
    }

    // =====================================================
    // BÓNG Ở PHÍA SAU
    // =====================================================

    if (m_player.isFacingRight())
    {
        if (ballX < playerX - 5.f)
        {
            m_input.left = true;
            return;
        }
    }
    else
    {
        if (ballX > playerX + 5.f)
        {
            m_input.right = true;
            return;
        }
    }

    // =====================================================
    // BÓNG Ở TRÊN CAO
    // =====================================================

    if (ballY < playerY - 60.f)
    {
        m_input.jump = true;

        // Không đứng trong KickBall mãi.
        m_recoverTimer = 0.10f;
        m_state = BotState::Recover;

        return;
    }

    // =====================================================
    // KICK
    // =====================================================

    m_input.kick = true;

    // Cho phép chuyển trạng thái sau một khoảng ngắn.
    m_recoverTimer = 0.18f;

    m_state = BotState::Recover;
}


// =========================================================
// RECOVER
// =========================================================

void FSMController::updateRecover(float deltaTime)
{
    m_recoverTimer -= deltaTime;

    // Trong lúc recover vẫn cho bot tiếp tục
    // hướng về phía bóng một chút.
    float playerX =
        m_player.getPosition().x;

    float ballX =
        m_ball.getPosition().x;

    float dx =
        ballX - playerX;

    // Không đứng im hoàn toàn sau khi kick.
    if (std::abs(dx) > 60.f)
    {
        if (dx > 0.f)
        {
            m_input.right = true;
        }
        else
        {
            m_input.left = true;
        }
    }

    if (m_recoverTimer <= 0.f)
    {
        m_state = BotState::ChaseBall;
    }
}


// =========================================================
// DEFEND
// =========================================================

void FSMController::updateDefend()
{
    float goalX =
        Config::AI_DEFEND_X_OFFSET;

    float playerX =
        m_player.getPosition().x;

    float playerY =
        m_player.getPosition().y;

    float ballX =
        m_ball.getPosition().x;

    float ballY =
        m_ball.getPosition().y;

    float dx =
        goalX - playerX;

    // =====================================================
    // DI CHUYỂN VỀ VỊ TRÍ PHÒNG THỦ
    // =====================================================

    if (dx > Config::AI_STOP_DISTANCE)
    {
        m_input.right = true;
    }
    else if (dx < -Config::AI_STOP_DISTANCE)
    {
        m_input.left = true;
    }

    // =====================================================
    // BÓNG Ở GẦN KHUNG THÀNH
    // =====================================================

    bool ballNearGoal =
        std::abs(goalX - ballX) < 130.f;

    // =====================================================
    // NHẢY CẢN BÓNG
    // =====================================================

    if (ballNearGoal &&
        ballY < playerY - 60.f)
    {
        m_input.jump = true;
    }

    // =====================================================
    // DASH PHÒNG THỦ
    // =====================================================
    //
    // Nếu bóng đang lao về phía khung thành
    // và Bot còn cách vị trí phòng thủ khá xa,
    // dùng dash để quay về nhanh.
    //

    float distanceToGoal =
        std::abs(goalX - playerX);

    sf::Vector2f ballVel =
        m_ball.getVelocity();

    bool ballComingFast =
        ballVel.x > 100.f;

    if (distanceToGoal > 120.f &&
        ballComingFast)
    {
        m_input.dash = true;
    }
}