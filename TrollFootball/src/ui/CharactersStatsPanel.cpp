#include "ui/CharacterStatsPanel.h"
#include "ui/CharacterSelectMenu.h"
#include <string>

CharacterStatsPanel::CharacterStatsPanel(const sf::Font& font, sf::Vector2f size)
    : m_nameText(font)
    , m_descriptionText(font)
    , m_statsText(font)
{
    m_background.setSize(size);
    m_background.setFillColor(sf::Color(20, 20, 20, 180));
    m_background.setOutlineThickness(2.f);
    m_background.setOutlineColor(sf::Color(120, 120, 120));

    m_nameText.setCharacterSize(24);
    m_nameText.setFillColor(sf::Color(255, 215, 0));

    m_descriptionText.setCharacterSize(16);
    m_descriptionText.setFillColor(sf::Color(220, 220, 220));

    m_statsText.setCharacterSize(18);
    m_statsText.setFillColor(sf::Color::White);
}

void CharacterStatsPanel::setPosition(sf::Vector2f position)
{
    m_background.setPosition(position);
    m_nameText.setPosition({ position.x + 20.f, position.y + 10.f });
    m_descriptionText.setPosition({ position.x + 20.f, position.y + 46.f });
    m_statsText.setPosition({ position.x + 20.f, position.y + 90.f });
}

void CharacterStatsPanel::setCharacter(const CharacterOption& character)
{
    m_nameText.setString(character.displayName);
    m_descriptionText.setString(character.description);

    std::string statsStr =
        "   Speed: " + std::to_string(character.speed) +
        "   Jump: " + std::to_string(character.jump) +
        "   Kick: " + std::to_string(character.kick) +
        "   Stun cooldown: " + std::to_string(character.stunCooldown);

    m_statsText.setString(statsStr);
    m_visible = true;
}

void CharacterStatsPanel::setVisible(bool visible)
{
    m_visible = visible;
}

void CharacterStatsPanel::draw(sf::RenderWindow& window) const
{
    if (!m_visible)
        return;

    window.draw(m_background);
    window.draw(m_nameText);
    window.draw(m_descriptionText);
    window.draw(m_statsText);
}