#pragma once

#include "entities/Player.h"
#include "entities/Ball.h"

enum class BotState
{
    Idle,
    ChaseBall,
    KickBall,
    Recover,
    Defend
};

class FSMController
{
public:

    FSMController(Player& player, Ball& ball);

    void update(float deltaTime);

    InputCommand getInput() const;

private:

    void updateIdle();
    void updateChaseBall();
    void updateKickBall();
    void updateRecover(float deltaTime);
    void updateDefend();

    Player& m_player;
    Ball& m_ball;

    BotState m_state{ BotState::Idle };

    InputCommand m_input;

    // Thời gian nghỉ sau mỗi cú đá
    float m_recoverTimer{ 0.f };
};