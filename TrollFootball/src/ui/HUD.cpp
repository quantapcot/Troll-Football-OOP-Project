#include "ui/HUD.h"

HUD::HUD(const sf::Font& font, const std::string& teamAName, const std::string& teamBName, float matchSeconds)
    : m_scoreBoard(font, teamAName, teamBName)
    , m_timer(font, matchSeconds)
{
}

void HUD::update(float deltaTime)
{
    m_timer.update(deltaTime);
}

void HUD::draw(sf::RenderWindow& window)
{
    m_scoreBoard.draw(window);
    m_timer.draw(window);
}

ScoreBoard& HUD::getScoreBoard()
{
    return m_scoreBoard;
}

Timer& HUD::getTimer()
{
    return m_timer;
}