#pragma once
#include <SFML/Graphics.hpp>

class Timer
{
public:
    Timer(const sf::Font& font, float totalSeconds = 90.f);

    void update(float deltaTime);

    void draw(sf::RenderWindow& window);

    void pause();
    void resume();

    bool isTimeUp() const;

    float getElapsedTime() const { return m_totalTime - m_timeLeft; }
    float getTotalTime() const { return m_totalTime; }

    void reset();

private:
    void updateDisplayText();

    float m_totalTime;
    float m_timeLeft;    // Thời gian còn lại (giây)
    bool  m_isPaused;    // true => không đếm giờ (game đang tạm dừng)

    sf::Text m_text;
};