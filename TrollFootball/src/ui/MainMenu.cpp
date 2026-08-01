#include "ui/MainMenu.h"
#include "audio/AudioManager.h"

MainMenu::MainMenu(const sf::Font& font, sf::Vector2u windowSize)
// SFML 3: mọi sf::Text member (m_title, m_playText, m_exitText khai báo trong MainMenu.h)
// đều PHẢI được khởi tạo với 1 Font ngay tại đây, vì không còn constructor mặc định nữa.
    : m_title(font)
    , m_playText(font)
    , m_exitText(font)
{
    // ----- Tieu de -----
    m_title.setString("HEAD FOOTBALL");
    m_title.setCharacterSize(64);
    m_title.setFillColor(sf::Color::White);

    // SFML 3: sf::FloatRect đổi cấu trúc từ (left, top, width, height)
    // sang 2 Vector2f: .position (x,y) và .size (width, height)
    // => bounds.width (2.x) trở thành bounds.size.x (3.x)
    float titleWidth = m_title.getLocalBounds().size.x;
    m_title.setPosition({ (windowSize.x - titleWidth) / 2.f, 100.f });

    // ----- Nut Play -----
    m_playButton.setSize(sf::Vector2f(220.f, 60.f));
    m_playButton.setFillColor(sf::Color(30, 150, 30));
    m_playButton.setPosition({ (windowSize.x - 220.f) / 2.f, 300.f });

    m_playText.setString("CHOI NGAY");
    m_playText.setCharacterSize(24);
    m_playText.setFillColor(sf::Color::White);
    m_playText.setPosition({
        m_playButton.getPosition().x + (m_playButton.getSize().x - m_playText.getLocalBounds().size.x) / 2.f,
        m_playButton.getPosition().y + 15.f
        });

    // ----- Nut Exit -----
    m_exitButton.setSize(sf::Vector2f(220.f, 60.f));
    m_exitButton.setFillColor(sf::Color(150, 30, 30));
    m_exitButton.setPosition({ (windowSize.x - 220.f) / 2.f, 390.f });

    m_exitText.setString("THOAT");
    m_exitText.setCharacterSize(24);
    m_exitText.setFillColor(sf::Color::White);
    m_exitText.setPosition({
        m_exitButton.getPosition().x + (m_exitButton.getSize().x - m_exitText.getLocalBounds().size.x) / 2.f,
        m_exitButton.getPosition().y + 15.f
        });
}

void MainMenu::updateHover(sf::Vector2f mousePos)
{
    if (m_playButton.getGlobalBounds().contains(mousePos))
        m_playButton.setFillColor(sf::Color(50, 200, 50));
    else
        m_playButton.setFillColor(sf::Color(30, 150, 30));

    if (m_exitButton.getGlobalBounds().contains(mousePos))
        m_exitButton.setFillColor(sf::Color(200, 50, 50));
    else
        m_exitButton.setFillColor(sf::Color(150, 30, 30));
}

MainMenuAction MainMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // SFML 3: sf::Event giờ là 1 lớp bọc std::variant của nhiều loại sự kiện con.
    // event.is<T>() kiểm tra event hiện tại có đúng kiểu T không (thay cho event.type == sf::Event::X)
    if (event.is<sf::Event::MouseMoved>())
    {
        updateHover(mousePos);
        return MainMenuAction::None;
    }

    // event.getIf<T>() trả về con trỏ tới dữ liệu chi tiết của sự kiện nếu đúng kiểu T,
    // trả về nullptr nếu không đúng kiểu => dùng if (auto* p = ...) để vừa kiểm tra vừa lấy dữ liệu
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        // SFML 3: sf::Mouse::Button giờ là enum class => phải viết đầy đủ sf::Mouse::Button::Left
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            if (m_playButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return MainMenuAction::Play;
            }
            if (m_exitButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return MainMenuAction::Exit;
            }
        }
    }

    return MainMenuAction::None;
}

void MainMenu::draw(sf::RenderWindow& window)
{
    window.draw(m_title);
    window.draw(m_playButton);
    window.draw(m_playText);
    window.draw(m_exitButton);
    window.draw(m_exitText);
}