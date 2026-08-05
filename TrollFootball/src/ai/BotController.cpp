#include "ai/BotController.h"

BotController::BotController(Player& player,
    Ball& ball)
    : m_player(player),
    m_fsm(player, ball)
{
}

void BotController::update(float deltaTime)
{
    m_fsm.update(deltaTime);

    m_player.setInput(
        m_fsm.getInput()
    );
}