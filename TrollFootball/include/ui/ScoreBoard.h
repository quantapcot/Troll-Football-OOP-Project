#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class ScoreBoard
{
public:
    ScoreBoard(const sf::Font& font, const std::string& teamAName, const std::string& teamBName);

    void addScoreA();
    void addScoreB();

    // Lấy điểm hiện tại
    int getScoreA() const;
    int getScoreB() const;

    void draw(sf::RenderWindow& window);

    // Reset tỉ số về 0-0
    void reset();

private:
    void updateDisplayText();

    std::string m_teamAName;
    std::string m_teamBName;

    int m_scoreA;
    int m_scoreB; 

    sf::Text m_text;
};