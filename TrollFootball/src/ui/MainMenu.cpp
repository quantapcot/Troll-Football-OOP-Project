#include "ui/MainMenu.h"
#include "audio/AudioManager.h"
#include <iostream>

MainMenu::MainMenu(const sf::Font& font, sf::Vector2u windowSize, const std::string& backgroundImagePath)
    : m_title(font)
    , m_vsBotText(font)
    , m_vsPlayerText(font)
    , m_exitText(font)
{
    // ----- MOI: Anh nen -----
    // Chi tao Sprite khi load Texture THANH CONG. Neu duong dan rong hoac load loi,
    // m_backgroundSprite van la std::nullopt => draw() se tu dong bo qua, khong crash.
    if (!backgroundImagePath.empty())
    {
        if (m_backgroundTexture.loadFromFile(backgroundImagePath))
        {
            // emplace() tao sf::Sprite MOI ben trong optional, bat buoc phai truyen Texture
            // ngay luc tao vi SFML 3 khong con constructor mac dinh cho sf::Sprite
            m_backgroundSprite.emplace(m_backgroundTexture);

            // Phong to/thu nho anh nen sao cho vua khit kich thuoc cua so,
            // bat ke anh goc to hay nho hon window
            sf::Vector2u textureSize = m_backgroundTexture.getSize();
            float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
            float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);
            m_backgroundSprite->setScale({ scaleX, scaleY }); // setScale SFML 3 chi nhan Vector2f
        }
        else
        {
            std::cerr << "[MainMenu] Khong the load anh nen: " << backgroundImagePath << std::endl;
        }
    }

    // ----- Tieu de -----
    m_title.setString("TROLL FOOTBALL");
    m_title.setCharacterSize(64);
    m_title.setFillColor(sf::Color::White);
    // MOI: them vien den mo phia sau chu de doc duoc ro rang tren anh nen (khong bi chim vao anh)
    m_title.setOutlineColor(sf::Color::Black);
    m_title.setOutlineThickness(3.f);

    float titleWidth = m_title.getLocalBounds().size.x;
    // SUA: dung ti le theo windowSize.y thay vi so cung 100.f, de menu con dep tren nhieu do phan giai
    m_title.setPosition({ (windowSize.x - titleWidth) / 2.f, windowSize.y * 0.12f });

    // ----- SUA: 3 nut xep doc, vi tri tinh theo ti le chieu cao cua so -----
    float buttonWidth = 260.f;
    float buttonHeight = 55.f;
    float centerX = (windowSize.x - buttonWidth) / 2.f;
    float firstButtonY = windowSize.y * 0.45f;
    float buttonSpacing = 75.f;

    // ----- Nut VS BOT (chua co co che, hien thi dang "sap ra mat") -----
    m_vsBotButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    m_vsBotButton.setFillColor(sf::Color(90, 90, 90)); // Mau xam = dang bi vo hieu hoa (disabled)
    m_vsBotButton.setPosition({ centerX, firstButtonY });

    m_vsBotText.setString("VS BOT (COMING SOON)");
    m_vsBotText.setCharacterSize(18); // Chu nho hon vi chuoi dai hon cac nut khac
    m_vsBotText.setFillColor(sf::Color(200, 200, 200));
    m_vsBotText.setPosition({
        centerX + (buttonWidth - m_vsBotText.getLocalBounds().size.x) / 2.f,
        firstButtonY + 16.f
        });

    // ----- Nut VS PLAYER (co che chinh, da hoat dong tu truoc) -----
    float secondButtonY = firstButtonY + buttonSpacing;
    m_vsPlayerButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    m_vsPlayerButton.setFillColor(sf::Color(30, 150, 30));
    m_vsPlayerButton.setPosition({ centerX, secondButtonY });

    m_vsPlayerText.setString("VS PLAYER");
    m_vsPlayerText.setCharacterSize(24);
    m_vsPlayerText.setFillColor(sf::Color::White);
    m_vsPlayerText.setPosition({
        centerX + (buttonWidth - m_vsPlayerText.getLocalBounds().size.x) / 2.f,
        secondButtonY + 15.f
        });

    // ----- Nut THOAT -----
    float thirdButtonY = secondButtonY + buttonSpacing;
    m_exitButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    m_exitButton.setFillColor(sf::Color(150, 30, 30));
    m_exitButton.setPosition({ centerX, thirdButtonY });

    m_exitText.setString("EXIT");
    m_exitText.setCharacterSize(24);
    m_exitText.setFillColor(sf::Color::White);
    m_exitText.setPosition({
        centerX + (buttonWidth - m_exitText.getLocalBounds().size.x) / 2.f,
        thirdButtonY + 15.f
        });
}

void MainMenu::updateHover(sf::Vector2f mousePos)
{
    // SUA: nut VS BOT dang disabled => KHONG doi mau khi hover, luon giu mau xam co dinh
    // (khong goi ham doi mau cho no o day nua, chi con 2 nut duoi la co hieu ung hover)

    if (m_vsPlayerButton.getGlobalBounds().contains(mousePos))
        m_vsPlayerButton.setFillColor(sf::Color(50, 200, 50));
    else
        m_vsPlayerButton.setFillColor(sf::Color(30, 150, 30));

    if (m_exitButton.getGlobalBounds().contains(mousePos))
        m_exitButton.setFillColor(sf::Color(200, 50, 50));
    else
        m_exitButton.setFillColor(sf::Color(150, 30, 30));
}

MainMenuAction MainMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.is<sf::Event::MouseMoved>())
    {
        updateHover(mousePos);
        return MainMenuAction::None;
    }

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            // MOI: nut VS BOT van BAM DUOC (tra ve PlayVsBot), nhung Game.cpp se tu quyet dinh
            // bo qua hanh dong nay vi che do Vs Bot chua duoc lam. Van phat am thanh click
            // de nguoi choi biet nut co phan hoi, khong bi "chet" hoan toan.
            if (m_vsBotButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return MainMenuAction::PlayVsBot;
            }
            if (m_vsPlayerButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return MainMenuAction::PlayVsPlayer;
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
    // MOI: ve anh nen TRUOC TIEN de no nam duoi cung, moi thu khac ve sau se nam de len tren
    if (m_backgroundSprite.has_value())
        window.draw(*m_backgroundSprite);

    window.draw(m_title);
    window.draw(m_vsBotButton);
    window.draw(m_vsBotText);
    window.draw(m_vsPlayerButton);
    window.draw(m_vsPlayerText);
    window.draw(m_exitButton);
    window.draw(m_exitText);
}