#include "ui/WinScreen.h"
#include "audio/AudioManager.h"

static void setupButton(sf::RectangleShape& box, sf::Text& text, const std::string& label,
    float x, float y, sf::Color color)
{
    box.setSize(sf::Vector2f(260.f, 55.f));
    box.setFillColor(color);
    box.setPosition({ x, y });

    text.setString(label);
    text.setCharacterSize(22);
    text.setFillColor(sf::Color::White);
    text.setPosition({
        x + (box.getSize().x - text.getLocalBounds().size.x) / 2.f,
        y + (box.getSize().y - text.getLocalBounds().size.y) / 2.f - 5.f
        });
}

WinScreen::WinScreen(const sf::Font& font, sf::Vector2u windowSize)
    : m_titleText(font)
    , m_scoreText(font)
    , m_restartText(font)
    , m_mainMenuText(font)
{
    m_overlay.setSize(sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
    m_overlay.setFillColor(sf::Color(0, 0, 0, 200));

    m_titleText.setString("VICTORY!");
    m_titleText.setCharacterSize(52);
    m_titleText.setFillColor(sf::Color::Yellow);
    float titleWidth = m_titleText.getLocalBounds().size.x;
    m_titleText.setPosition({ (windowSize.x - titleWidth) / 2.f, 130.f });

    m_scoreText.setCharacterSize(30);
    m_scoreText.setFillColor(sf::Color::White);
    m_scoreText.setPosition({ (windowSize.x - 100.f) / 2.f, 210.f });

    float centerX = (windowSize.x - 260.f) / 2.f;
    setupButton(m_restartButton, m_restartText, "PLAY AGAIN", centerX, 300.f, sf::Color(30, 150, 30));
    setupButton(m_mainMenuButton, m_mainMenuText, "MAIN MENU", centerX, 370.f, sf::Color(60, 60, 150));
}

void WinScreen::setFinalScore(int myScore, int opponentScore)
{
    std::string display = "TI SO: " + std::to_string(myScore) + " - " + std::to_string(opponentScore);
    m_scoreText.setString(display);

    float textWidth = m_scoreText.getLocalBounds().size.x;
    m_scoreText.setPosition({ (m_overlay.getSize().x - textWidth) / 2.f, 210.f });
}

void WinScreen::setWinnerLabel(const std::string& label)
{
    // MOI: thay noi dung tieu de bang ten nguoi thang cu the, thay vi luon la "CHIEN THANG!" chung chung
    m_titleText.setString(label);

    // Can lai giua man hinh vi do dai chuoi thay doi tuy label ("PLAYER 1 THANG!" khac do dai voi "HOA!")
    float titleWidth = m_titleText.getLocalBounds().size.x;
    m_titleText.setPosition({ (m_overlay.getSize().x - titleWidth) / 2.f, 130.f });
}

void WinScreen::updateHover(sf::Vector2f mousePos)
{
    auto hoverColor = [&](sf::RectangleShape& box, sf::Color base)
        {
            if (box.getGlobalBounds().contains(mousePos))
                box.setFillColor(sf::Color(base.r + 20, base.g + 20, base.b + 20));
            else
                box.setFillColor(base);
        };

    hoverColor(m_restartButton, sf::Color(30, 150, 30));
    hoverColor(m_mainMenuButton, sf::Color(60, 60, 150));
}

WinScreenAction WinScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.is<sf::Event::MouseMoved>())
    {
        updateHover(mousePos);
        return WinScreenAction::None;
    }

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            if (m_restartButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return WinScreenAction::Restart;
            }
            if (m_mainMenuButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return WinScreenAction::MainMenu;
            }
        }
    }

    return WinScreenAction::None;
}

void WinScreen::draw(sf::RenderWindow& window)
{
    window.draw(m_overlay);
    window.draw(m_titleText);
    window.draw(m_scoreText);
    window.draw(m_restartButton);
    window.draw(m_restartText);
    window.draw(m_mainMenuButton);
    window.draw(m_mainMenuText);
}