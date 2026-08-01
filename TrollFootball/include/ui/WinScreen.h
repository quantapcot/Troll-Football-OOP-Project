#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class WinScreenAction
{
    None,
    Restart,
    MainMenu
};

// Màn hình hiện ra khi người chơi THẮNG trận
// Cấu trúc gần như giống hệt GameOver, nhưng tách class riêng vì:
// 1. Nội dung/màu sắc khác nhau hoàn toàn (chiến thắng vs thất bại)
// 2. Sau này dễ thêm hiệu ứng riêng cho từng màn hình (confetti khi thắng, không cần khi thua...)
//    mà không sợ ảnh hưởng lẫn nhau
class WinScreen
{
public:
    WinScreen(const sf::Font& font, sf::Vector2u windowSize);

    void setFinalScore(int myScore, int opponentScore);

    WinScreenAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    void updateHover(sf::Vector2f mousePos);

    sf::RectangleShape m_overlay;

    sf::Text m_titleText;  // "CHIEN THANG!"
    sf::Text m_scoreText;

    sf::RectangleShape m_restartButton;
    sf::Text m_restartText;

    sf::RectangleShape m_mainMenuButton;
    sf::Text m_mainMenuText;
};