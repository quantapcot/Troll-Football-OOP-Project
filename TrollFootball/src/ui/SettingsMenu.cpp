#include "ui/SettingsMenu.h"
#include "audio/AudioManager.h"
#include <iostream>

SettingsMenu::SettingsMenu(const sf::Font& font, sf::Vector2u windowSize)
    : m_titleText(font)
    , m_rulesText(font)
    , m_muteText(font)
    , m_volDownText(font)
    , m_volUpText(font)
    , m_volValueText(font)
    , m_backText(font)
{

    float w = static_cast<float>(windowSize.x);
    float h = static_cast<float>(windowSize.y);

    // Overlay den mo
    m_overlay.setSize({ w, h });
    m_overlay.setFillColor(sf::Color(0, 0, 0, 180));

    // Khung Panel trung tam
    float panelW = 500.f;
    float panelH = 460.f;
    float panelX = (w - panelW) / 2.f;
    float panelY = (h - panelH) / 2.f;

    m_panel.setSize({ panelW, panelH });
    m_panel.setFillColor(sf::Color(30, 40, 60, 230));
    m_panel.setOutlineColor(sf::Color(255, 215, 0));
    m_panel.setOutlineThickness(3.f);
    m_panel.setPosition({ panelX, panelY });

    // Title "CAI DAT" / "SETTINGS"
    m_titleText.setString("SETTINGS");
    m_titleText.setCharacterSize(42);
    m_titleText.setFillColor(sf::Color::Yellow);
    m_titleText.setOutlineColor(sf::Color::Black);
    m_titleText.setOutlineThickness(2.f);
    float titleW = m_titleText.getLocalBounds().size.x;
    m_titleText.setPosition({ panelX + (panelW - titleW) / 2.f, panelY + 25.f });

    float btnW = 320.f;
    float btnH = 50.f;
    float btnX = panelX + (panelW - btnW) / 2.f;
    float startY = panelY + 100.f;

    // 1. Nut LUAT CHOI (RULES)
    m_rulesButton.setSize({ btnW, btnH });
    m_rulesButton.setFillColor(sf::Color(150, 30, 30));
    m_rulesButton.setPosition({ btnX, startY });

    m_rulesText.setString("RULES");
    m_rulesText.setCharacterSize(22);
    m_rulesText.setFillColor(sf::Color::White);
    float rTextW = m_rulesText.getLocalBounds().size.x;
    m_rulesText.setPosition({ btnX + (btnW - rTextW) / 2.f, startY + 12.f });

    // 2. Nut TAT/BAT AM THANH (MUTE TOGGLE)
    float muteY = startY + 65.f;
    m_muteButton.setSize({ btnW, btnH });
    m_muteButton.setFillColor(sf::Color(150, 30, 30));
    m_muteButton.setPosition({ btnX, muteY });

    m_muteText.setString("SOUND ON");
    m_muteText.setCharacterSize(22);
    m_muteText.setFillColor(sf::Color::White);
    float mTextW = m_muteText.getLocalBounds().size.x;
    m_muteText.setPosition({ btnX + (btnW - mTextW) / 2.f, muteY + 12.f });

    // 3. CHINH AM LUONG (-) (+)
    float volY = muteY + 65.f;

    float stepBtnW = 45.f;
    m_volDownButton.setSize({ stepBtnW, btnH });
    m_volDownButton.setFillColor(sf::Color(150, 30, 30));
    m_volDownButton.setPosition({ btnX, volY });

    m_volDownText.setString("-");
    m_volDownText.setCharacterSize(28);
    m_volDownText.setFillColor(sf::Color::White);
    m_volDownText.setPosition({ btnX + 15.f, volY + 6.f });

    m_volUpButton.setSize({ stepBtnW, btnH });
    m_volUpButton.setFillColor(sf::Color(150, 30, 30));
    m_volUpButton.setPosition({ btnX + btnW - stepBtnW, volY });

    m_volUpText.setString("+");
    m_volUpText.setCharacterSize(28);
    m_volUpText.setFillColor(sf::Color::White);
    m_volUpText.setPosition({ btnX + btnW - stepBtnW + 12.f, volY + 6.f });

    m_volValueText.setString("VOLUME: 100%");
    m_volValueText.setCharacterSize(20);
    m_volValueText.setFillColor(sf::Color::White);
    float vTextW = m_volValueText.getLocalBounds().size.x;
    m_volValueText.setPosition({ btnX + (btnW - vTextW) / 2.f, volY + 14.f });

    // 4. Nut QUAY LAI (BACK)
    float backY = volY + 80.f;
    m_backButton.setSize({ btnW, btnH });
    m_backButton.setFillColor(sf::Color(80, 80, 80));
    m_backButton.setPosition({ btnX, backY });

    m_backText.setString("BACK");
    m_backText.setCharacterSize(22);
    m_backText.setFillColor(sf::Color::White);
    float bTextW = m_backText.getLocalBounds().size.x;
    m_backText.setPosition({ btnX + (btnW - bTextW) / 2.f, backY + 12.f });
}

void SettingsMenu::updateState(bool isMuted, float volumeLevel)
{
    if (isMuted)
    {
        m_muteText.setString("SOUND OFF");
    }
    else
    {
        m_muteText.setString("SOUND ON");
    }
    float mTextW = m_muteText.getLocalBounds().size.x;
    float btnX = m_muteButton.getPosition().x;
    float btnW = m_muteButton.getSize().x;
    float muteY = m_muteButton.getPosition().y;
    m_muteText.setPosition({ btnX + (btnW - mTextW) / 2.f, muteY + 12.f });

    int volPercent = static_cast<int>(volumeLevel);
    m_volValueText.setString("VOLUME: " + std::to_string(volPercent) + "%");
    float vTextW = m_volValueText.getLocalBounds().size.x;
    float volY = m_volDownButton.getPosition().y;
    m_volValueText.setPosition({ btnX + (btnW - vTextW) / 2.f, volY + 14.f });
}

void SettingsMenu::updateHover(sf::Vector2f mousePos)
{
    auto checkHover = [](sf::RectangleShape& btn, sf::Vector2f pos, sf::Color normal, sf::Color hover) {
        if (btn.getGlobalBounds().contains(pos))
            btn.setFillColor(hover);
        else
            btn.setFillColor(normal);
    };

    checkHover(m_rulesButton, mousePos, sf::Color(150, 30, 30), sf::Color(200, 50, 50));
    checkHover(m_muteButton, mousePos, sf::Color(150, 30, 30), sf::Color(200, 50, 50));
    checkHover(m_volDownButton, mousePos, sf::Color(150, 30, 30), sf::Color(200, 50, 50));
    checkHover(m_volUpButton, mousePos, sf::Color(150, 30, 30), sf::Color(200, 50, 50));
    checkHover(m_backButton, mousePos, sf::Color(80, 80, 80), sf::Color(120, 120, 120));
}

SettingsMenuAction SettingsMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.is<sf::Event::MouseMoved>())
    {
        updateHover(mousePos);
        return SettingsMenuAction::None;
    }

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            if (m_rulesButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return SettingsMenuAction::Rules;
            }
            if (m_muteButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return SettingsMenuAction::ToggleMute;
            }
            if (m_volDownButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return SettingsMenuAction::VolumeDown;
            }
            if (m_volUpButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return SettingsMenuAction::VolumeUp;
            }
            if (m_backButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return SettingsMenuAction::Back;
            }
        }
    }

    return SettingsMenuAction::None;
}

void SettingsMenu::draw(sf::RenderWindow& window)
{
    window.draw(m_overlay);
    window.draw(m_panel);
    window.draw(m_titleText);

    window.draw(m_rulesButton);
    window.draw(m_rulesText);

    window.draw(m_muteButton);
    window.draw(m_muteText);

    window.draw(m_volDownButton);
    window.draw(m_volDownText);
    window.draw(m_volUpButton);
    window.draw(m_volUpText);
    window.draw(m_volValueText);

    window.draw(m_backButton);
    window.draw(m_backText);
}
