#include "ui/PauseMenu.h"
#include "audio/AudioManager.h"

static void setupButton(sf::RectangleShape& box, sf::Text& text, const std::string& label,
    float x, float y, sf::Color color)
{
    // Lưu ý: text đã được các constructor bên ngoài khởi tạo với Font từ trước
    // (vì sf::Text không còn ctor mặc định), hàm này chỉ chỉnh nội dung/vị trí thôi.
    box.setSize(sf::Vector2f(260.f, 55.f));
    box.setFillColor(color);
    box.setPosition({ x, y }); // setPosition SFML 3 chỉ nhận Vector2f

    text.setString(label);
    text.setCharacterSize(22);
    text.setFillColor(sf::Color::White);
    text.setPosition({
        x + (box.getSize().x - text.getLocalBounds().size.x) / 2.f,
        y + (box.getSize().y - text.getLocalBounds().size.y) / 2.f - 5.f
        });
}

PauseMenu::PauseMenu(const sf::Font& font, sf::Vector2u windowSize)
// Khởi tạo mọi sf::Text member với Font ngay tại initializer list (bắt buộc ở SFML 3)
    : m_titleText(font)
    , m_resumeText(font)
    , m_restartText(font)
    , m_mainMenuText(font)
{
    m_overlay.setSize(sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)));
    m_overlay.setFillColor(sf::Color(0, 0, 0, 180));

    m_titleText.setString("PAUSE");
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color::White);
    float titleWidth = m_titleText.getLocalBounds().size.x; // .size.x thay cho .width
    m_titleText.setPosition({ (windowSize.x - titleWidth) / 2.f, 120.f });

    float centerX = (windowSize.x - 260.f) / 2.f;

    setupButton(m_resumeButton, m_resumeText, "RESUME", centerX, 250.f, sf::Color(30, 150, 30));
    setupButton(m_restartButton, m_restartText, "RESTART", centerX, 320.f, sf::Color(150, 130, 30));
    setupButton(m_mainMenuButton, m_mainMenuText, "MAIN MENU", centerX, 390.f, sf::Color(150, 30, 30));
}

void PauseMenu::updateHover(sf::Vector2f mousePos)
{
    auto hoverColor = [&](sf::RectangleShape& box, sf::Color base)
        {
            if (box.getGlobalBounds().contains(mousePos))
                box.setFillColor(sf::Color(base.r + 20, base.g + 20, base.b + 20));
            else
                box.setFillColor(base);
        };

    hoverColor(m_resumeButton, sf::Color(30, 150, 30));
    hoverColor(m_restartButton, sf::Color(150, 130, 30));
    hoverColor(m_mainMenuButton, sf::Color(150, 30, 30));
}

PauseMenuAction PauseMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.is<sf::Event::MouseMoved>())
    {
        updateHover(mousePos);
        return PauseMenuAction::None;
    }

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            if (m_resumeButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return PauseMenuAction::Resume;
            }
            if (m_restartButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return PauseMenuAction::Restart;
            }
            if (m_mainMenuButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return PauseMenuAction::MainMenu;
            }
        }
    }

    return PauseMenuAction::None;
}

void PauseMenu::draw(sf::RenderWindow& window)
{
    window.draw(m_overlay);
    window.draw(m_titleText);
    window.draw(m_resumeButton);
    window.draw(m_resumeText);
    window.draw(m_restartButton);
    window.draw(m_restartText);
    window.draw(m_mainMenuButton);
    window.draw(m_mainMenuText);
}