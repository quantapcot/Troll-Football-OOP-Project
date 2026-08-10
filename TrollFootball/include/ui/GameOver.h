#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <ui/IScreen.h>

enum class GameOverAction
{
    None,
    Restart,
    MainMenu
};

// Màn hình hiện ra khi người chơi THUA trận
class GameOver : public IScreen
{
public:
    GameOver(const sf::Font& font, sf::Vector2u windowSize);

    // Gọi hàm này khi trận đấu kết thúc và xác định được người chơi thua
    // để hiển thị đúng tỉ số cuối cùng, ví dụ "BAN THUA 1 - 3"
    void setFinalScore(int myScore, int opponentScore);

    GameOverAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) override;

private:
    void updateHover(sf::Vector2f mousePos);

    sf::RectangleShape m_overlay;

    sf::Text m_titleText;  // "BAN THUA CUOC"
    sf::Text m_scoreText;  // Hiển thị tỉ số cuối trận

    sf::RectangleShape m_restartButton;
    sf::Text m_restartText;

    sf::RectangleShape m_mainMenuButton;
    sf::Text m_mainMenuText;
};