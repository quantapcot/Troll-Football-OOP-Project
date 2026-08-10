#pragma once
#include <SFML/Graphics.hpp>
#include <ui/IScreen.h>

enum class PauseMenuAction
{
    None,
    Resume, 
    Restart,  
    MainMenu 
};

class PauseMenu : public IScreen
{
public:
    PauseMenu(const sf::Font& font, sf::Vector2u windowSize);

    PauseMenuAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    void draw(sf::RenderWindow& window) override;

private:
    void updateHover(sf::Vector2f mousePos);

    sf::RectangleShape m_overlay;

    sf::Text m_titleText;

    sf::RectangleShape m_resumeButton;
    sf::Text m_resumeText;

    sf::RectangleShape m_restartButton;
    sf::Text m_restartText;

    sf::RectangleShape m_mainMenuButton;
    sf::Text m_mainMenuText;
};