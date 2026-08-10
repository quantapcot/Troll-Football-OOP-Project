#pragma once
#include <SFML/Graphics.hpp>
#include "ui/ScoreBoard.h"
#include "ui/Timer.h"

// HUD (Heads-Up Display) là lớp giao diện luôn hiển thị trong lúc trận đấu diễn ra
class HUD
{
public:
    HUD(const sf::Font& font, const std::string& teamAName, const std::string& teamBName, float matchSeconds = 90.f);

    void update(float deltaTime);

    void draw(sf::RenderWindow& window);

    ScoreBoard& getScoreBoard();
    Timer& getTimer();

private:
    ScoreBoard m_scoreBoard;
    Timer m_timer;         
};