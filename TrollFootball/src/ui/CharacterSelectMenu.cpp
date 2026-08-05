#include "ui/CharacterSelectMenu.h"
#include "core/AssetManager.h"
#include "audio/AudioManager.h"
#include <algorithm>

CharacterSelectMenu::CharacterSelectMenu(const sf::Font& font,
    sf::Vector2u windowSize,
    std::vector<CharacterOption> characters)
    : m_characters(std::move(characters))
    , m_title(font)
    , m_playText(font)
    , m_backText(font)
{
    // TITLE
    m_title.setString("CHOOSE YOUR GOAT");
    m_title.setCharacterSize(48);
    m_title.setFillColor(sf::Color::White);
    m_title.setOutlineColor(sf::Color::Black);
    m_title.setOutlineThickness(3.f);

    float titleWidth = m_title.getLocalBounds().size.x;
    m_title.setPosition({ (windowSize.x - titleWidth) / 2.f, windowSize.y * 0.10f });

    // ----- PLAYER CARDS -----
    constexpr float cardSize = 140.f;
    constexpr float cardSpacing = 40.f;
    constexpr float cardPadding = 16.f;

    float totalWidth = m_characters.size() * cardSize
        + (m_characters.empty() ? 0.f : (m_characters.size() - 1) * cardSpacing);

    float startX = (windowSize.x - totalWidth) / 2.f;
    float cardY = windowSize.y * 0.32f;

    m_cards.reserve(m_characters.size());

    for (std::size_t i = 0; i < m_characters.size(); ++i)
    {
        const auto& texture = AssetManager::get().getTexture(m_characters[i].textureKey);

        Card card(texture, font);

        float cardX = startX + static_cast<float>(i) * (cardSize + cardSpacing);
        card.frame.setSize({ cardSize, cardSize });
        card.frame.setPosition({ cardX, cardY });
        card.frame.setFillColor(sf::Color(40, 40, 40, 200));
        card.frame.setOutlineThickness(4.f);
        card.frame.setOutlineColor(sf::Color(100, 100, 100));

        //PREVIEW IMAGE
        auto texSize = texture.getSize();
        float maxDim = cardSize - cardPadding * 2.f;
        float scale = std::min(
            maxDim / static_cast<float>(texSize.x),
            maxDim / static_cast<float>(texSize.y));

        card.sprite.setOrigin({ texSize.x / 2.f, texSize.y / 2.f });
        card.sprite.setScale({ scale, scale });
        card.sprite.setPosition({
            cardX + cardSize / 2.f,
            cardY + cardSize / 2.f
            });

        //CHARACTER NAME
        card.nameText.setString(m_characters[i].displayName);
        card.nameText.setCharacterSize(20);
        card.nameText.setFillColor(sf::Color::White);

        float nameWidth = card.nameText.getLocalBounds().size.x;
        card.nameText.setPosition({
            cardX + (cardSize - nameWidth) / 2.f,
            cardY + cardSize + 8.f
            });

        m_cards.push_back(std::move(card));
    }

	//DEFAULT SELECTED INDEX:
    m_selectedIndex = m_characters.empty() ? -1 : 0;

    //PLAY BUTTON
    float buttonWidth = 200.f;
    float buttonHeight = 55.f;

    m_playButton.setSize({ buttonWidth, buttonHeight });
    m_playButton.setFillColor(sf::Color(30, 130, 30));
    m_playButton.setPosition({
        (windowSize.x - buttonWidth) / 2.f,
        windowSize.y * 0.75f
        });

    m_playText.setString("PLAY ");
    m_playText.setCharacterSize(26);
    m_playText.setFillColor(sf::Color::White);

    float playTextWidth = m_playText.getLocalBounds().size.x;
    m_playText.setPosition({
        m_playButton.getPosition().x + (buttonWidth - playTextWidth) / 2.f,
        m_playButton.getPosition().y + 12.f
        });

    // ----- Nut QUAY LAI -----
    m_backButton.setSize({ 160.f, 50.f });
    m_backButton.setFillColor(sf::Color(150, 30, 30));
    m_backButton.setPosition({
        40.f,
        windowSize.y * 0.75f + 3.f
        });

    m_backText.setString("BACK");
    m_backText.setCharacterSize(22);
    m_backText.setFillColor(sf::Color::White);

    float backTextWidth = m_backText.getLocalBounds().size.x;
    m_backText.setPosition({
        m_backButton.getPosition().x + (m_backButton.getSize().x - backTextWidth) / 2.f,
        m_backButton.getPosition().y + 10.f
        });
}

void CharacterSelectMenu::updateHover(sf::Vector2f mousePos)
{
    for (std::size_t i = 0; i < m_cards.size(); ++i)
    {
        bool hovered = m_cards[i].frame.getGlobalBounds().contains(mousePos);
        bool selected = (static_cast<int>(i) == m_selectedIndex);

        if (selected)
            m_cards[i].frame.setOutlineColor(sf::Color::Yellow);
        else if (hovered)
            m_cards[i].frame.setOutlineColor(sf::Color(200, 200, 200));
        else
            m_cards[i].frame.setOutlineColor(sf::Color(100, 100, 100));
    }

    if (m_playButton.getGlobalBounds().contains(mousePos))
        m_playButton.setFillColor(sf::Color(50, 180, 50));
    else
        m_playButton.setFillColor(sf::Color(30, 130, 30));

    if (m_backButton.getGlobalBounds().contains(mousePos))
        m_backButton.setFillColor(sf::Color(200, 50, 50));
    else
        m_backButton.setFillColor(sf::Color(150, 30, 30));
}

CharacterSelectAction CharacterSelectMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.is<sf::Event::MouseMoved>())
    {
        updateHover(mousePos);
        return CharacterSelectAction::None;
    }

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            for (std::size_t i = 0; i < m_cards.size(); ++i)
            {
                if (m_cards[i].frame.getGlobalBounds().contains(mousePos))
                {
                    m_selectedIndex = static_cast<int>(i);
                    AudioManager::getInstance().playSound("button");
                    return CharacterSelectAction::None;
                }
            }

            if (m_selectedIndex >= 0 && m_playButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return CharacterSelectAction::Confirm;
            }

            if (m_backButton.getGlobalBounds().contains(mousePos))
            {
                AudioManager::getInstance().playSound("button");
                return CharacterSelectAction::Back;
            }
        }
    }

    return CharacterSelectAction::None;
}

void CharacterSelectMenu::draw(sf::RenderWindow& window)
{
    window.draw(m_title);

    for (auto& card : m_cards)
    {
        window.draw(card.frame);
        window.draw(card.sprite);
        window.draw(card.nameText);
    }

    window.draw(m_playButton);
    window.draw(m_playText);

    window.draw(m_backButton);
    window.draw(m_backText);
}