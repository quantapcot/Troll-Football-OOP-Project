#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <ui/IScreen.h>

enum class MainMenuAction
{
    None,
    PlayVsBot,
    PlayVsPlayer,
    CharacterSelect,   
    Settings,         
    Exit
};

class MainMenu : public IScreen
{
public:
    MainMenu(const sf::Font& font, sf::Vector2u windowSize,
        const std::string& backgroundImagePath = "",
        const std::string& settingsIconPath = "");

    MainMenuAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    void draw(sf::RenderWindow& window) override;

private:
    void updateHover(sf::Vector2f mousePos);

    sf::Texture m_backgroundTexture;
    std::optional<sf::Sprite> m_backgroundSprite;

    sf::Texture m_settingsIconTexture;
    std::optional<sf::Sprite> m_settingsIconSprite;

    sf::Text m_title;

    sf::RectangleShape m_vsBotButton;
    sf::Text m_vsBotText;

    sf::RectangleShape m_vsPlayerButton;
    sf::Text m_vsPlayerText;

    // MOI: nut rieng cho man hinh xem/chon nhan vat
    sf::RectangleShape m_characterSelectButton;
    sf::Text m_characterSelectText;

    // MOI: nut cai dat hinh vuong o goc man hinh
    sf::RectangleShape m_settingsButton;
    sf::Text m_settingsText;

    sf::RectangleShape m_exitButton;
    sf::Text m_exitText;
};