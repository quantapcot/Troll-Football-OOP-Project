#include "ui/CharacterStatsPanel.h"
#include "ui/CharacterSelectMenu.h"
#include <string>

CharacterStatsPanel::CharacterStatsPanel(const sf::Font& font, sf::Vector2f size)
    : m_nameText(font)
    , m_descriptionText(font)
    , m_statsLabelText(font)
    , m_statsValueText(font)
{
    // Cố định chiều cao panel khoảng 260px để không bị tràn chữ
    m_background.setSize({ size.x, 260.f });
    m_background.setFillColor(sf::Color(20, 20, 20, 180));
    m_background.setOutlineThickness(2.f);
    m_background.setOutlineColor(sf::Color(120, 120, 120));

    m_nameText.setCharacterSize(24);
    m_nameText.setFillColor(sf::Color(255, 215, 0));

    m_descriptionText.setCharacterSize(16);
    m_descriptionText.setFillColor(sf::Color(220, 220, 220));

    m_statsLabelText.setCharacterSize(18);
    m_statsLabelText.setFillColor(sf::Color::White);
    
    m_statsValueText.setCharacterSize(18);
    m_statsValueText.setFillColor(sf::Color(100, 255, 100)); // Màu xanh lá cho giá trị nổi bật
}

void CharacterStatsPanel::setPosition(sf::Vector2f position)
{
    m_background.setPosition(position);
    m_nameText.setPosition({ position.x + 20.f, position.y + 10.f });
    m_descriptionText.setPosition({ position.x + 20.f, position.y + 46.f });
    
    // Cột Label và Cột Value tách nhau 110px
    m_statsLabelText.setPosition({ position.x + 20.f, position.y + 100.f });
    m_statsValueText.setPosition({ position.x + 130.f, position.y + 100.f });
}

void CharacterStatsPanel::setCharacter(const CharacterOption& character)
{
    m_nameText.setString(character.displayName);
    m_descriptionText.setString(character.description);

    auto makeBar = [](int value) {
        std::string bar = "";
        for (int i = 0; i < value; ++i) bar += "\xE2\x96\x88"; // UTF-8 cho █
        return bar + "  " + std::to_string(value) + "/10";
    };

    std::string labelStr = 
        "Speed:\n\n"
        "Jump:\n\n"
        "Kick:\n\n"
        "Recovery:";

    std::string valueStr = 
        makeBar(character.speed) + "\n\n" +
        makeBar(character.jump) + "\n\n" +
        makeBar(character.kick) + "\n\n" +
        makeBar(character.stunCooldown);

    m_statsLabelText.setString(labelStr);
    m_statsValueText.setString(sf::String::fromUtf8(valueStr.begin(), valueStr.end()));
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
    window.draw(m_statsLabelText);
    window.draw(m_statsValueText);
}