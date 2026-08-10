#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <ui/IScreen.h>

enum class SettingsMenuAction
{
    None,
    Rules,      
    ToggleMute,  
    VolumeDown,  
    VolumeUp,    
    Back          
};

class SettingsMenu : public IScreen
{
public:
    SettingsMenu(const sf::Font& font, sf::Vector2u windowSize);

    SettingsMenuAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void updateState(bool isMuted, float volumeLevel);
    void draw(sf::RenderWindow& window) override;

private:
    void updateHover(sf::Vector2f mousePos);

    sf::RectangleShape m_overlay;
    sf::RectangleShape m_panel;

    sf::Text m_titleText;

    sf::RectangleShape m_rulesButton;
    sf::Text m_rulesText;

    sf::RectangleShape m_muteButton;
    sf::Text m_muteText;

    sf::RectangleShape m_volDownButton;
    sf::Text m_volDownText;

    sf::RectangleShape m_volUpButton;
    sf::Text m_volUpText;

    sf::Text m_volValueText;

    sf::RectangleShape m_backButton;
    sf::Text m_backText;
};
