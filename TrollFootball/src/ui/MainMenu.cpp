#include "ui/MainMenu.h"
#include "audio/AudioManager.h"
#include <iostream>

MainMenu::MainMenu(const sf::Font& font, sf::Vector2u windowSize, const std::string& backgroundImagePath, const std::string& settingsIconPath)
    : m_title(font)
    , m_vsBotText(font)
    , m_vsPlayerText(font)
    , m_characterSelectText(font)
    , m_settingsText(font)
    , m_exitText(font)
{
    //BACKGROUND
    if (!backgroundImagePath.empty())
    {
        if (m_backgroundTexture.loadFromFile(backgroundImagePath))
        {
            m_backgroundSprite.emplace(m_backgroundTexture);

            // Phong to/thu nho anh nen sao cho vua khit kich thuoc cua so,
            // bat ke anh goc to hay nho hon window
            sf::Vector2u textureSize = m_backgroundTexture.getSize();
            float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
            float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);
            m_backgroundSprite->setScale({ scaleX, scaleY });
        }
        else
        {
            std::cerr << "[MainMenu] Khong the load anh nen: " << backgroundImagePath << std::endl;
        }
    }

    //TITLE
    m_title.setString("TROLL FOOTBALL");
    m_title.setCharacterSize(64);
    m_title.setFillColor(sf::Color::White);
    //BORDER
    m_title.setOutlineColor(sf::Color::Black);
    m_title.setOutlineThickness(3.f);

    float titleWidth = m_title.getLocalBounds().size.x;
    // SUA: dung ti le theo windowSize.y thay vi so cung 100.f, de menu con dep tren nhieu do phan giai
    m_title.setPosition({ (windowSize.x - titleWidth) / 2.f, windowSize.y * 0.12f });

	//SETTINGS BUTTON (HINH VUONG GOC MAN HINH)
    float sqSize = 52.f;
    float sqX = windowSize.x - sqSize - 25.f;
    float sqY = 25.f;
    m_settingsButton.setSize({ sqSize, sqSize });
    m_settingsButton.setFillColor(sf::Color(150, 30, 30));
    m_settingsButton.setOutlineColor(sf::Color(255, 215, 0));
    m_settingsButton.setOutlineThickness(2.f);
    m_settingsButton.setPosition({ sqX, sqY });

    if (!settingsIconPath.empty())
    {
        if (m_settingsIconTexture.loadFromFile(settingsIconPath))
        {
            m_settingsIconSprite.emplace(m_settingsIconTexture);

            auto iconSize = m_settingsIconTexture.getSize();

            // Dat origin vao giua icon de de dat vao chinh giua nut hinh vuong
            m_settingsIconSprite->setOrigin({
                iconSize.x / 2.f,
                iconSize.y / 2.f
                });

            // Scale icon nho hon kich thuoc nut mot chut (chua padding quanh icon)
            float iconPadding = 10.f;
            float maxIconDim = sqSize - iconPadding * 2.f;
            float scale = std::min(
                maxIconDim / static_cast<float>(iconSize.x),
                maxIconDim / static_cast<float>(iconSize.y));

            m_settingsIconSprite->setScale({ scale, scale });

            // Dat vi tri vao chinh giua nut Settings
            m_settingsIconSprite->setPosition({
                sqX + sqSize / 2.f,
                sqY + sqSize / 2.f
                });
        }
        else
        {
            std::cerr << "[MainMenu] Khong the load icon settings: " << settingsIconPath << std::endl;
        }
    }

	//BUTTON SCALE
    float buttonWidth = 260.f;
    float buttonHeight = 55.f;
    float centerX = (windowSize.x - buttonWidth) / 2.f;
    float firstButtonY = windowSize.y * 0.40f;
    float buttonSpacing = 68.f;

    //VS BOT BUTTON
    m_vsBotButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    m_vsBotButton.setFillColor(sf::Color(150, 30, 30));
    m_vsBotButton.setPosition({ centerX, firstButtonY });

    m_vsBotText.setString("VS BOT");
    m_vsBotText.setCharacterSize(24);
    m_vsBotText.setFillColor(sf::Color::White);
    m_vsBotText.setPosition({
        centerX + (buttonWidth - m_vsBotText.getLocalBounds().size.x) / 2.f,
        firstButtonY + 16.f
        });

    //VS PLAYER BUTTON
    float secondButtonY = firstButtonY + buttonSpacing;
    m_vsPlayerButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    m_vsPlayerButton.setFillColor(sf::Color(150, 30, 30));
    m_vsPlayerButton.setPosition({ centerX, secondButtonY });

    m_vsPlayerText.setString("VS PLAYER");
    m_vsPlayerText.setCharacterSize(24);
    m_vsPlayerText.setFillColor(sf::Color::White);
    m_vsPlayerText.setPosition({
        centerX + (buttonWidth - m_vsPlayerText.getLocalBounds().size.x) / 2.f,
        secondButtonY + 15.f
        });

    //CHOOSE CHARACTER
    float thirdButtonY = secondButtonY + buttonSpacing;
    m_characterSelectButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    m_characterSelectButton.setFillColor(sf::Color(150, 30, 30));
    m_characterSelectButton.setPosition({ centerX, thirdButtonY });
    m_characterSelectText.setString("CHARACTERS");
    m_characterSelectText.setCharacterSize(24);
    m_characterSelectText.setFillColor(sf::Color::White);
    m_characterSelectText.setPosition({
        centerX + (buttonWidth - m_characterSelectText.getLocalBounds().size.x) / 2.f,
        thirdButtonY + 15.f
        });

    //EXIT
    float fourthButtonY = thirdButtonY + buttonSpacing;
    m_exitButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    m_exitButton.setFillColor(sf::Color(150, 30, 30));
    m_exitButton.setPosition({ centerX, fourthButtonY });

    m_exitText.setString("EXIT");
    m_exitText.setCharacterSize(24);
    m_exitText.setFillColor(sf::Color::White);
    m_exitText.setPosition({
        centerX + (buttonWidth - m_exitText.getLocalBounds().size.x) / 2.f,
        fourthButtonY + 15.f
        });
}

void MainMenu::updateHover(sf::Vector2f mousePos)
{
    if (m_vsBotButton.getGlobalBounds().contains(mousePos))
        m_vsBotButton.setFillColor(sf::Color(200, 50, 50));
    else
        m_vsBotButton.setFillColor(sf::Color(150, 30, 30));

    if (m_vsPlayerButton.getGlobalBounds().contains(mousePos))
        m_vsPlayerButton.setFillColor(sf::Color(200, 50, 50));
    else
        m_vsPlayerButton.setFillColor(sf::Color(150, 30, 30));

    if (m_characterSelectButton.getGlobalBounds().contains(mousePos))
        m_characterSelectButton.setFillColor(sf::Color(200, 50, 50));
    else
        m_characterSelectButton.setFillColor(sf::Color(150, 30, 30));

    if (m_settingsButton.getGlobalBounds().contains(mousePos))
        m_settingsButton.setFillColor(sf::Color(200, 50, 50));
    else
        m_settingsButton.setFillColor(sf::Color(150, 30, 30));

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

            if (m_characterSelectButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return MainMenuAction::CharacterSelect;
            }
            if (m_settingsButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return MainMenuAction::Settings;
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
    if (m_backgroundSprite.has_value())
        window.draw(*m_backgroundSprite);
    window.draw(m_title);
    window.draw(m_vsBotButton);
    window.draw(m_vsBotText);
    window.draw(m_vsPlayerButton);
    window.draw(m_vsPlayerText);
    window.draw(m_characterSelectButton);   
    window.draw(m_characterSelectText);     
    window.draw(m_settingsButton);
    if (m_settingsIconSprite.has_value())
        window.draw(*m_settingsIconSprite);
    window.draw(m_exitButton);
    window.draw(m_exitText);
}