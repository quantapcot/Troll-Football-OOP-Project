#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <ui/IScreen.h>

enum class WinScreenAction
{
    None,
    Restart,
    MainMenu
};

class WinScreen : public IScreen
{
public:
    WinScreen(const sf::Font& font, sf::Vector2u windowSize);

    void setFinalScore(int myScore, int opponentScore);

	// NAME OF THE WINNER (PLAYER 1 or PLAYER 2)
    void setWinnerLabel(const std::string& label);

    WinScreenAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) override;

private:
    void updateHover(sf::Vector2f mousePos);

    sf::RectangleShape m_overlay;

    sf::Text m_titleText;  // "VICTORY!"
    sf::Text m_scoreText;

    sf::RectangleShape m_restartButton;
    sf::Text m_restartText;

    sf::RectangleShape m_mainMenuButton;
    sf::Text m_mainMenuText;
};