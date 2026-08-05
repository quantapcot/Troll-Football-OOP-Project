#pragma once

#include "entities/Player.h"
#include "entities/Ball.h"
#include "ai/FSMController.h"

class BotController
{
public:

    BotController(Player& player, Ball& ball);

    void update(float deltaTime);

private:

    Player& m_player;

    FSMController m_fsm;
};