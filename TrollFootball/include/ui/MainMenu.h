#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

// SUA: them 2 lua chon rieng cho che do choi, thay vi chi co "Play" chung chung
enum class MainMenuAction
{
    None,        // Chua bam gi ca
    PlayVsBot,   // Bam nut "VS BOT" - CHUA CO CO CHE, Game.cpp se tam thoi bo qua hanh dong nay
    PlayVsPlayer,// Bam nut "VS PLAYER" - co che 2 nguoi choi da lam tu truoc, hoat dong binh thuong
    Exit         // Bam nut "THOAT"
};

class MainMenu
{
public:
    // MOI: backgroundImagePath - duong dan anh nen (vd "assets/images/menu_bg.png")
    // Truyen chuoi rong "" neu khong muon dung anh nen (menu se chi co mau nen mac dinh cua window)
    MainMenu(const sf::Font& font, sf::Vector2u windowSize, const std::string& backgroundImagePath = "");

    MainMenuAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);

private:
    void updateHover(sf::Vector2f mousePos);

    // MOI: anh nen - Texture giu du lieu anh, Sprite la doi tuong ve len man hinh tu Texture do
    // std::optional vi sf::Sprite (SFML 3) khong co constructor mac dinh, phai co Texture truoc
    sf::Texture m_backgroundTexture;
    std::optional<sf::Sprite> m_backgroundSprite;

    sf::Text m_title;

    // SUA: doi ten m_playButton/m_playText thanh m_vsPlayerButton/m_vsPlayerText cho ro nghia,
    // va them cap nut moi cho che do Vs Bot
    sf::RectangleShape m_vsBotButton;
    sf::Text m_vsBotText;

    sf::RectangleShape m_vsPlayerButton;
    sf::Text m_vsPlayerText;

    sf::RectangleShape m_exitButton;
    sf::Text m_exitText;
};