#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class RulesScreenAction
{
    None,
    Back   // Quay lai Settings Menu
};

class RulesScreen
{
public:
    RulesScreen(const sf::Font& font, sf::Vector2u windowSize);

    void loadRulesFromFile(const std::string& filePath = "assets/textures/law/law.txt");

    RulesScreenAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    void updateHover(sf::Vector2f mousePos);

    sf::RectangleShape m_overlay;
    sf::RectangleShape m_panel;

    sf::Text m_titleText;
    sf::Text m_rulesBodyText;

    sf::RectangleShape m_backButton;
    sf::Text m_backText;
};
