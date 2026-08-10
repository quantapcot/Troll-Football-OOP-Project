#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <ui/IScreen.h>

enum class RulesScreenAction
{
    None,
    Back   // Quay lai Settings Menu
};

class RulesScreen : public IScreen
{
public:
    RulesScreen(const sf::Font& font, sf::Vector2u windowSize);

    void loadRulesFromFile(const std::string& filePath = "assets/textures/law/law.txt");

    RulesScreenAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) override;

private:
    void updateHover(sf::Vector2f mousePos);
    void clampScroll();
    void updateScrollbar();

    sf::Vector2u m_windowSize;

    sf::RectangleShape m_overlay;
    sf::RectangleShape m_panel;

    sf::Text m_titleText;
    sf::Text m_rulesBodyText;

    sf::RectangleShape m_backButton;
    sf::Text m_backText;

    // Scrollable panel properties
    float m_contentX{ 0.f };
    float m_contentY{ 0.f };
    float m_contentW{ 0.f };
    float m_contentH{ 0.f };

    float m_scrollOffset{ 0.f };
    float m_maxScroll{ 0.f };

    // Scrollbar UI
    sf::RectangleShape m_scrollbarTrack;
    sf::RectangleShape m_scrollbarThumb;
    float m_thumbH{ 40.f };
    bool m_isDraggingScrollbar{ false };
    float m_dragMouseStartY{ 0.f };
    float m_dragStartScrollOffset{ 0.f };
};
