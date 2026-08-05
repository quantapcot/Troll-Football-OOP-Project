#include "ai/FSMController.h"

#include <cmath>

constexpr float KICK_DISTANCE = 45.f;
constexpr float STOP_DISTANCE = 12.f;
constexpr float JUMP_HEIGHT = 80.f;
constexpr float JUMP_DISTANCE = 80.f;
constexpr float DEFEND_X = Config::WINDOW_WIDTH - 220.f;
constexpr float DASH_DISTANCE = 280.f;

FSMController::FSMController(Player& player,
    Ball& ball)
    : m_player(player),
    m_ball(ball)
{
}

void FSMController::update(float deltaTime)
{
    m_input = {};

    float ballX = m_ball.getPosition().x;

    // Nếu bóng ở gần khung thành mình thì ưu tiên phòng thủ
    if (ballX > Config::WINDOW_WIDTH * 0.7f)
    {
        m_state = BotState::Defend;
    }
    else if (m_state == BotState::Defend)
    {
        m_state = BotState::ChaseBall;
    }

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

void FSMController::updateIdle()
{
    m_state = BotState::ChaseBall;
}

void FSMController::updateChaseBall()
{
    float playerX = m_player.getPosition().x;
    float playerY = m_player.getPosition().y;

    auto ballPos = m_ball.getPosition();
    auto ballVel = m_ball.getVelocity();

    float ballX =
        ballPos.x +
        ballVel.x * 0.22f;

    float ballY =
        ballPos.y +
        ballVel.y * 0.12f;

    float targetX;

    // Player đỏ luôn nhìn phải
    if (m_player.isFacingRight())
    {
        targetX = ballX - 35.f;
    }
    // Player xanh luôn nhìn trái
    else
    {
        targetX = ballX + 35.f;
    }

    float dx = targetX - playerX;
    float dy = ballY - playerY;

    float distance =
        std::sqrt(
            (ballPos.x - playerX) * (ballPos.x - playerX) +
            (ballPos.y - playerY) * (ballPos.y - playerY));

    // =========================
    // ĐỦ GẦN -> ĐÁ
    // =========================

    if (distance <= KICK_DISTANCE)
    {
        m_state = BotState::KickBall;
        return;
    }

    // =========================
    // DI CHUYỂN THEO TRỤC X
    // =========================

    if (dx > STOP_DISTANCE)
    {
        m_input.right = true;
    }
    else if (dx < -STOP_DISTANCE)
    {
        m_input.left = true;
    }

    // =========================
    // JUMP
    // =========================

    // Chỉ nhảy khi bóng ở khá gần và đang ở trên đầu

    if (std::abs(dx) < JUMP_DISTANCE &&
        dy < -JUMP_HEIGHT)
    {
        m_input.jump = true;
    }

    // =========================
    // DASH
    // =========================

    if (distance > DASH_DISTANCE &&
        std::abs(dy) < 80.f &&
        std::abs(ballVel.x) > 60.f &&
        m_state != BotState::Defend)
    {
        m_input.dash = true;
    }
}

void FSMController::updateKickBall()
{
    float playerX = m_player.getPosition().x;
    float ballX = m_ball.getPosition().x;
    float playerY = m_player.getPosition().y;
    float ballY = m_ball.getPosition().y;

    // =========================
    // Căn vị trí trước khi đá
    // =========================

    if (m_player.isFacingRight())
    {
        // Nếu bóng vẫn ở sau lưng thì lùi lại
        if (ballX < playerX + 15.f)
        {
            m_input.left = true;
            return;
        }
    }
    else
    {
        // Nếu bóng vẫn ở sau lưng thì tiến thêm
        if (ballX > playerX - 15.f)
        {
            m_input.right = true;
            return;
        }
    }

    // =========================
    // Đá
    // =========================

    // Nếu bóng quá cao thì nhảy trước
    if (ballY < playerY - 60.f)
    {
        m_input.jump = true;
        return;
    }

    m_input.kick = true;

    // Nghỉ một chút sau khi đá
    m_recoverTimer = 0.25f;

    m_state = BotState::Recover;
}

void FSMController::updateRecover(float deltaTime)
{
    m_recoverTimer -= deltaTime;

    if (m_recoverTimer <= 0.f)
    {
        m_state = BotState::ChaseBall;
    }
}

void FSMController::updateDefend()
{
    float goalX = DEFEND_X;

    float playerX = m_player.getPosition().x;
    float playerY = m_player.getPosition().y;

    float ballY = m_ball.getPosition().y;

    if (std::abs(goalX - playerX) > 10.f)
    {
        if (playerX < goalX)
            m_input.right = true;
        else
            m_input.left = true;
    }

    if (playerX < goalX)
        m_input.right = true;
    else
        m_input.left = true;

    // Nếu bóng bay cao ngay trước khung thành thì nhảy cản
    if (std::abs(goalX - m_ball.getPosition().x) < 90.f &&
        ballY < playerY - 70.f)
    {
        m_input.jump = true;
    }
}