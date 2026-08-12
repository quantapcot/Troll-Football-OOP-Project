#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>
#include <ui/IScreen.h>
#include <ui/CharacterStatsPanel.h>


enum class CharacterSelectAction
{
    None,
    Back,
    Confirm
};

//Information of character option, including display name and texture key for rendering
struct CharacterOption
{
    std::string displayName;
    std::string textureKey;
    int speed = 0;
    int jump = 0;
    int kick = 0;
    int stunCooldown = 0;
    std::string description;
};

class CharacterSelectMenu : public IScreen
{
public:
    CharacterSelectMenu(const sf::Font& font,
        sf::Vector2u windowSize,
        std::vector<CharacterOption> characters,
        const std::string& backgroundImagePath = "");

    CharacterSelectAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    void draw(sf::RenderWindow& window) override;

    const CharacterOption& getSelectedCharacter() const
    {
        return m_characters[static_cast<std::size_t>(m_selectedIndex)];
    }

	//Get the opponent character option different from the selected character
    const CharacterOption& getOpponentCharacter() const
    {
        std::size_t opponentIndex =
            (static_cast<std::size_t>(m_selectedIndex) + 1) % m_characters.size();

        return m_characters[opponentIndex];
    }
    
    void setTitle(const std::string& text);
    void setPlayButtonLabel(const std::string& text);


private:
    void updateHover(sf::Vector2f mousePos);

    sf::Texture m_backgroundTexture;
    std::optional<sf::Sprite> m_backgroundSprite;

    struct Card
    {
        sf::Sprite sprite;
        sf::RectangleShape frame;
        sf::Text nameText;

        Card(const sf::Texture& texture, const sf::Font& font)
            : sprite(texture), nameText(font)
        {
        }
    };

    std::vector<CharacterOption> m_characters;
    std::vector<Card> m_cards;
    int m_selectedIndex{ -1 };

    sf::Text m_title;

    sf::RectangleShape m_playButton;
    sf::Text m_playText;

    sf::RectangleShape m_backButton;
    sf::Text m_backText;

    sf::Vector2u m_windowSize;

    CharacterStatsPanel m_statsPanel;
};