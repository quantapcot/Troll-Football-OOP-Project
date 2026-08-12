#pragma once

#include <SFML/Graphics.hpp>
#include <string>

struct CharacterOption;

// Hien thi ten, mo ta va chi so cua mot nhan vat duoc chon
class CharacterStatsPanel
{
public:
    CharacterStatsPanel(const sf::Font& font, sf::Vector2f size);

    // Cap nhat noi dung panel theo nhan vat duoc truyen vao
    void setCharacter(const CharacterOption& character);

    // An/hien panel (vi du khi chua co nhan vat nao duoc chon)
    void setVisible(bool visible);

    void draw(sf::RenderWindow& window) const;

    void setPosition(sf::Vector2f position);

private:
    bool m_visible{ false };

    sf::RectangleShape m_background;
    sf::Text m_nameText;
    sf::Text m_descriptionText;
    sf::Text m_statsText;
};