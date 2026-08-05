#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

enum class MainMenuAction
{
    None,
    PlayVsBot,
    PlayVsPlayer,
    CharacterSelect,   // MOI: nut rieng - chi de xem/chon nhan vat truoc, KHONG vao tran
    Exit
};

class MainMenu
{
public:
    MainMenu(const sf::Font& font, sf::Vector2u windowSize, const std::string& backgroundImagePath = "");

    MainMenuAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    void updateHover(sf::Vector2f mousePos);

    sf::Texture m_backgroundTexture;
    std::optional<sf::Sprite> m_backgroundSprite;

    sf::Text m_title;

    sf::RectangleShape m_vsBotButton;
    sf::Text m_vsBotText;

    sf::RectangleShape m_vsPlayerButton;
    sf::Text m_vsPlayerText;

    // MOI: nut rieng cho man hinh xem/chon nhan vat
    sf::RectangleShape m_characterSelectButton;
    sf::Text m_characterSelectText;

    sf::RectangleShape m_exitButton;
    sf::Text m_exitText;
};