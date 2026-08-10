#include "ui/RulesScreen.h"
#include "audio/AudioManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

RulesScreen::RulesScreen(const sf::Font& font, sf::Vector2u windowSize)
    : m_windowSize(windowSize)
    , m_titleText(font)
    , m_rulesBodyText(font)
    , m_backText(font)
{
    float w = static_cast<float>(windowSize.x);
    float h = static_cast<float>(windowSize.y);

    // Overlay den mo
    m_overlay.setSize({ w, h });
    m_overlay.setFillColor(sf::Color(0, 0, 0, 190));

    // Khung Panel hien thi luat
    float panelW = 780.f;
    float panelH = 580.f;
    float panelX = (w - panelW) / 2.f;
    float panelY = (h - panelH) / 2.f;

    m_panel.setSize({ panelW, panelH });
    m_panel.setFillColor(sf::Color(25, 35, 50, 240));
    m_panel.setOutlineColor(sf::Color(255, 215, 0));
    m_panel.setOutlineThickness(3.f);
    m_panel.setPosition({ panelX, panelY });

    // Title "RULES" (Cố định ở phía trên)
    m_titleText.setString("RULES");
    m_titleText.setCharacterSize(34);
    m_titleText.setFillColor(sf::Color::Yellow);
    m_titleText.setOutlineColor(sf::Color::Black);
    m_titleText.setOutlineThickness(2.f);
    float titleW = m_titleText.getLocalBounds().size.x;
    m_titleText.setPosition({ panelX + (panelW - titleW) / 2.f, panelY + 18.f });

    // Kích thước và vị trí vùng viewport cuộn nội dung
    m_contentX = panelX + 35.f;
    m_contentY = panelY + 70.f;
    m_contentW = panelW - 85.f;
    m_contentH = panelH - 145.f; // ~435.f

    // Rules Body Text (Kích thước chữ 16px để dễ đọc, không ép nhỏ)
    m_rulesBodyText.setCharacterSize(16);
    m_rulesBodyText.setFillColor(sf::Color::White);
    m_rulesBodyText.setPosition({ 0.f, 0.f });

    // Thanh Scrollbar bên phải
    float trackX = panelX + panelW - 25.f;
    float trackY = m_contentY;
    float trackW = 8.f;
    float trackH = m_contentH;

    m_scrollbarTrack.setSize({ trackW, trackH });
    m_scrollbarTrack.setPosition({ trackX, trackY });
    m_scrollbarTrack.setFillColor(sf::Color(40, 50, 70, 180));
    m_scrollbarTrack.setOutlineColor(sf::Color(100, 110, 130, 100));
    m_scrollbarTrack.setOutlineThickness(1.f);

    m_scrollbarThumb.setSize({ trackW, 40.f });
    m_scrollbarThumb.setPosition({ trackX, trackY });
    m_scrollbarThumb.setFillColor(sf::Color(255, 215, 0, 200));

    // Load file luat choi
    loadRulesFromFile();

    // Nut BACK (Cố định ở phía dưới)
    float btnW = 200.f;
    float btnH = 45.f;
    float btnX = panelX + (panelW - btnW) / 2.f;
    float btnY = panelY + panelH - 60.f;

    m_backButton.setSize({ btnW, btnH });
    m_backButton.setFillColor(sf::Color(150, 30, 30));
    m_backButton.setPosition({ btnX, btnY });

    m_backText.setString("BACK");
    m_backText.setCharacterSize(22);
    m_backText.setFillColor(sf::Color::White);
    float bTextW = m_backText.getLocalBounds().size.x;
    m_backText.setPosition({ btnX + (btnW - bTextW) / 2.f, btnY + 10.f });
}

void RulesScreen::clampScroll()
{
    if (m_scrollOffset < 0.f)
        m_scrollOffset = 0.f;
    if (m_scrollOffset > m_maxScroll)
        m_scrollOffset = m_maxScroll;

    updateScrollbar();
}

void RulesScreen::updateScrollbar()
{
    float textHeight = m_rulesBodyText.getLocalBounds().size.y;
    m_maxScroll = std::max(0.f, textHeight - m_contentH + 25.f);

    if (m_maxScroll <= 0.f)
    {
        m_thumbH = m_contentH;
        m_scrollbarThumb.setSize({ m_scrollbarTrack.getSize().x, m_thumbH });
        m_scrollbarThumb.setPosition(m_scrollbarTrack.getPosition());
    }
    else
    {
        m_thumbH = std::max(40.f, m_contentH * (m_contentH / textHeight));
        m_scrollbarThumb.setSize({ m_scrollbarTrack.getSize().x, m_thumbH });

        float trackScrollableDist = m_contentH - m_thumbH;
        float thumbY = m_contentY + trackScrollableDist * (m_scrollOffset / m_maxScroll);
        m_scrollbarThumb.setPosition({ m_scrollbarTrack.getPosition().x, thumbY });
    }
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
        content = "GAME MODES\n"
                  "--------------------------------------------------\n"
                  "1. PLAY VS AI\n"
                  "- Player 1 plays against the computer-controlled opponent.\n\n"
                  "2. PLAY VS PLAYER\n"
                  "- Two human players play against each other.\n\n"
                  "--------------------------------------------------\n"
                  "CHARACTER SELECTION\n"
                  "--------------------------------------------------\n"
                  "- Before the match starts, players can select their characters.\n\n"
                  "--------------------------------------------------\n"
                  "CONTROLS\n"
                  "--------------------------------------------------\n"
                  "PLAYER 1:\n"
                  "- Move Left  : A\n"
                  "- Move Right : D\n"
                  "- Jump       : W\n"
                  "- Kick       : J\n"
                  "- Dash       : C\n\n"
                  "PLAYER 2:\n"
                  "- Move Left  : Left Arrow\n"
                  "- Move Right : Right Arrow\n"
                  "- Jump       : Up Arrow\n"
                  "- Kick       : Numpad 0\n"
                  "- Dash       : Numpad 1\n\n"
                  "--------------------------------------------------\n"
                  "MATCH RULES\n"
                  "--------------------------------------------------\n"
                  "- Each match lasts 1 minute 30 seconds (90 seconds).\n"
                  "- The player who scores the most goals when the time runs out wins.\n"
                  "- If both players have the same number of goals, the match ends in a draw.\n"
                  "- Random meteors can fall during the match and temporarily stun players.\n"
                  "- Players must avoid the falling meteors while trying to score goals.\n\n"
                  "--------------------------------------------------\n"
                  "PAUSE\n"
                  "--------------------------------------------------\n"
                  "- Press ESC during the match to open the Pause Menu.\n"
                  "- The Pause Menu allows the player to resume or leave the match.";
    }

    m_rulesBodyText.setString(content);
    m_scrollOffset = 0.f;
    updateScrollbar();
}

void RulesScreen::updateHover(sf::Vector2f mousePos)
{
    if (m_backButton.getGlobalBounds().contains(mousePos))
        m_backButton.setFillColor(sf::Color(200, 50, 50));
    else
        m_backButton.setFillColor(sf::Color(150, 30, 30));

    if (m_isDraggingScrollbar || m_scrollbarThumb.getGlobalBounds().contains(mousePos))
        m_scrollbarThumb.setFillColor(sf::Color(255, 235, 100, 255));
    else
        m_scrollbarThumb.setFillColor(sf::Color(255, 215, 0, 200));
}

RulesScreenAction RulesScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // Cuộn bằng con lăn chuột (Mouse Wheel)
    if (const auto* wheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>())
    {
        m_scrollOffset -= wheelScrolled->delta * 35.f;
        clampScroll();
        return RulesScreenAction::None;
    }

    // Cuộn bằng phím Mũi tên Up / Down
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->code == sf::Keyboard::Key::Up)
        {
            m_scrollOffset -= 40.f;
            clampScroll();
            return RulesScreenAction::None;
        }
        else if (keyPressed->code == sf::Keyboard::Key::Down)
        {
            m_scrollOffset += 40.f;
            clampScroll();
            return RulesScreenAction::None;
        }
    }

    // Xử lý di chuột và Kéo (Drag) thanh scrollbar
    if (event.is<sf::Event::MouseMoved>())
    {
        updateHover(mousePos);

        if (m_isDraggingScrollbar)
        {
            float deltaY = mousePos.y - m_dragMouseStartY;
            float trackScrollableDist = m_contentH - m_thumbH;
            if (trackScrollableDist > 0.f && m_maxScroll > 0.f)
            {
                float scrollChange = (deltaY / trackScrollableDist) * m_maxScroll;
                m_scrollOffset = m_dragStartScrollOffset + scrollChange;
                clampScroll();
            }
        }
        return RulesScreenAction::None;
    }

    // Click chuột
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            if (m_scrollbarThumb.getGlobalBounds().contains(mousePos))
            {
                m_isDraggingScrollbar = true;
                m_dragMouseStartY = mousePos.y;
                m_dragStartScrollOffset = m_scrollOffset;
                return RulesScreenAction::None;
            }
            else if (m_scrollbarTrack.getGlobalBounds().contains(mousePos))
            {
                float clickRatio = (mousePos.y - m_contentY) / m_contentH;
                m_scrollOffset = clickRatio * m_maxScroll;
                clampScroll();
                return RulesScreenAction::None;
            }

            if (m_backButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return RulesScreenAction::Back;
            }
        }
    }

    // Nhả chuột
    if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseReleased->button == sf::Mouse::Button::Left)
        {
            m_isDraggingScrollbar = false;
        }
    }

    return RulesScreenAction::None;
}

void RulesScreen::draw(sf::RenderWindow& window)
{
    // 1. Vẽ Overlay và Panel chính
    window.draw(m_overlay);
    window.draw(m_panel);

    // 2. Vẽ Tiêu đề "RULES" cố định ở trên
    window.draw(m_titleText);

    // 3. Render nội dung chữ được CLIP trong vùng Viewport nội dung
    float w = static_cast<float>(m_windowSize.x);
    float h = static_cast<float>(m_windowSize.y);

    sf::FloatRect viewportRect(
        { m_contentX / w, m_contentY / h },
        { m_contentW / w, m_contentH / h }
    );

    sf::View contentView(sf::FloatRect({ 0.f, m_scrollOffset }, { m_contentW, m_contentH }));
    contentView.setViewport(viewportRect);

    window.setView(contentView);
    m_rulesBodyText.setPosition({ 0.f, 0.f });
    window.draw(m_rulesBodyText);

    // Khôi phục View mặc định của Window
    window.setView(window.getDefaultView());

    // 4. Vẽ Thanh Scrollbar ở bên phải (nếu nội dung dài vượt chiều cao vùng view)
    if (m_maxScroll > 0.f)
    {
        window.draw(m_scrollbarTrack);
        window.draw(m_scrollbarThumb);
    }

    // 5. Vẽ Nút "BACK" cố định ở dưới
    window.draw(m_backButton);
    window.draw(m_backText);
}
