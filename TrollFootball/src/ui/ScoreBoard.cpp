#include "ui/ScoreBoard.h"
#include "audio/AudioManager.h"

ScoreBoard::ScoreBoard(const sf::Font& font, const std::string& teamAName, const std::string& teamBName)
    : m_teamAName(teamAName)
    , m_teamBName(teamBName)
    , m_scoreA(0)
    , m_scoreB(0)
    , m_text(font) // Bắt buộc truyền Font ở initializer list vì sf::Text không còn ctor mặc định
{
    m_text.setCharacterSize(28);
    m_text.setFillColor(sf::Color::Yellow);
    m_text.setPosition({ 20.f, 20.f }); // setPosition SFML 3 chỉ nhận Vector2f

    updateDisplayText();
}

void ScoreBoard::addScoreA()
{
    m_scoreA++;
    AudioManager::getInstance().playSound("goal");
    updateDisplayText();
}

void ScoreBoard::addScoreB()
{
    m_scoreB++;
    AudioManager::getInstance().playSound("goal");
    updateDisplayText();
}

int ScoreBoard::getScoreA() const { return m_scoreA; }
int ScoreBoard::getScoreB() const { return m_scoreB; }

void ScoreBoard::updateDisplayText()
{
    std::string display = m_teamAName + "  " + std::to_string(m_scoreA)
        + "  -  " + std::to_string(m_scoreB) + "  " + m_teamBName;

    m_text.setString(display);
}

void ScoreBoard::draw(sf::RenderWindow& window)
{
    window.draw(m_text);
}

void ScoreBoard::reset()
{
    m_scoreA = 0;
    m_scoreB = 0;
    updateDisplayText();
}