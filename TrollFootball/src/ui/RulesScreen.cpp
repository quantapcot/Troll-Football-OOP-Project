#include "ui/RulesScreen.h"
#include "audio/AudioManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

RulesScreen::RulesScreen(const sf::Font& font, sf::Vector2u windowSize)
    : m_titleText(font)
    , m_rulesBodyText(font)
    , m_backText(font)
{
    float w = static_cast<float>(windowSize.x);
    float h = static_cast<float>(windowSize.y);

    // Overlay den mo
    m_overlay.setSize({ w, h });
    m_overlay.setFillColor(sf::Color(0, 0, 0, 190));

    // Khung Panel hien thi luat
    float panelW = 750.f;
    float panelH = 580.f;
    float panelX = (w - panelW) / 2.f;
    float panelY = (h - panelH) / 2.f;

    m_panel.setSize({ panelW, panelH });
    m_panel.setFillColor(sf::Color(25, 35, 50, 240));
    m_panel.setOutlineColor(sf::Color(255, 215, 0));
    m_panel.setOutlineThickness(3.f);
    m_panel.setPosition({ panelX, panelY });

    // Title "LUAT CHOI" / "GAME RULES"
    m_titleText.setString("LUAT CHOI (GAME RULES)");
    m_titleText.setCharacterSize(34);
    m_titleText.setFillColor(sf::Color::Yellow);
    m_titleText.setOutlineColor(sf::Color::Black);
    m_titleText.setOutlineThickness(2.f);
    float titleW = m_titleText.getLocalBounds().size.x;
    m_titleText.setPosition({ panelX + (panelW - titleW) / 2.f, panelY + 20.f });

    // Rules Body Text
    m_rulesBodyText.setCharacterSize(16);
    m_rulesBodyText.setFillColor(sf::Color::White);
    m_rulesBodyText.setPosition({ panelX + 30.f, panelY + 75.f });

    // Load file luat choi
    loadRulesFromFile();

    // Nut BACK
    float btnW = 200.f;
    float btnH = 45.f;
    float btnX = panelX + (panelW - btnW) / 2.f;
    float btnY = panelY + panelH - 65.f;

    m_backButton.setSize({ btnW, btnH });
    m_backButton.setFillColor(sf::Color(150, 30, 30));
    m_backButton.setPosition({ btnX, btnY });

    m_backText.setString("BACK");
    m_backText.setCharacterSize(22);
    m_backText.setFillColor(sf::Color::White);
    float bTextW = m_backText.getLocalBounds().size.x;
    m_backText.setPosition({ btnX + (btnW - bTextW) / 2.f, btnY + 10.f });
}

void RulesScreen::loadRulesFromFile(const std::string& filePath)
{
    std::vector<std::string> pathsToTry = {
        filePath,
        "assets/textures/law/law.txt",
    };

    std::string content = "";
    bool loaded = false;

    for (const auto& path : pathsToTry)
    {
        std::ifstream file(path);
        if (file.is_open())
        {
            std::stringstream ss;
            ss << file.rdbuf();
            content = ss.str();
            loaded = true;
            std::cout << "[RulesScreen] Loaded rules from: " << path << std::endl;
            break;
        }
    }

    if (!loaded)
    {
        content = "LUAT CHOI TROLL FOOTBALL\n\n"
                  "1. MUC TIEU:\n"
                  "   Sut bong vao luoi doi phuong de ghi diem.\n\n"
                  "2. DIEU KHIEN:\n"
                  "   - Player 1: A / D (Di chuyen), W (Nhay), J (Sut)\n"
                  "   - Player 2: Left / Right (Di chuyen), Up (Nhay), Numpad 0 (Sut)\n\n"
                  "3. HIEN THI:\n"
                  "   - Dung nut ESC de Pause tro choi.";
    }

    m_rulesBodyText.setString(content);
}

void RulesScreen::updateHover(sf::Vector2f mousePos)
{
    if (m_backButton.getGlobalBounds().contains(mousePos))
        m_backButton.setFillColor(sf::Color(200, 50, 50));
    else
        m_backButton.setFillColor(sf::Color(150, 30, 30));
}

RulesScreenAction RulesScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.is<sf::Event::MouseMoved>())
    {
        updateHover(mousePos);
        return RulesScreenAction::None;
    }

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            if (m_backButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return RulesScreenAction::Back;
            }
        }
    }

    return RulesScreenAction::None;
}

void RulesScreen::draw(sf::RenderWindow& window)
{
    window.draw(m_overlay);
    window.draw(m_panel);
    window.draw(m_titleText);
    window.draw(m_rulesBodyText);

    window.draw(m_backButton);
    window.draw(m_backText);
}
