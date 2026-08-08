#pragma once
#include <SFML/Graphics.hpp>

enum class SettingsMenuAction
{
    None,
    Rules,        // Xem luat choi
    ToggleMute,   // Tat / bat am thanh
    VolumeDown,   // Giam am luong
    VolumeUp,     // Tang am luong
    Back          // Quay lai Main Menu
};

class SettingsMenu
{
public:
    SettingsMenu(const sf::Font& font, sf::Vector2u windowSize);

    SettingsMenuAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void updateState(bool isMuted, float volumeLevel);
    void draw(sf::RenderWindow& window);

private:
    void updateHover(sf::Vector2f mousePos);

    sf::RectangleShape m_overlay;
    sf::RectangleShape m_panel;

    sf::Text m_titleText;

    // Nut 1: Luat choi
    sf::RectangleShape m_rulesButton;
    sf::Text m_rulesText;

    // Nut 2: Tat/Bat am thanh
    sf::RectangleShape m_muteButton;
    sf::Text m_muteText;

    // Nut 2b: Chinh am luong (-) (+)
    sf::RectangleShape m_volDownButton;
    sf::Text m_volDownText;

    sf::RectangleShape m_volUpButton;
    sf::Text m_volUpText;

    sf::Text m_volValueText;

    // Nut 3: Quay lai
    sf::RectangleShape m_backButton;
    sf::Text m_backText;
};
