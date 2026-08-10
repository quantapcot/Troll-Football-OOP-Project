#pragma once
#include <SFML/Graphics.hpp>
#include <ui/IScreen.h>

enum class PauseMenuAction
{
    None,
    Resume,   // Tiếp tục chơi
    Restart,  // Chơi lại từ đầu
    MainMenu  // Quay về màn hình chính
};

// PauseMenu là 1 lớp phủ (overlay) hiển thị ĐÈ LÊN màn hình gameplay khi người chơi bấm Pause
// Vì vậy nó cần 1 nền mờ (semi-transparent) để người chơi vẫn thấy mờ mờ trận đấu phía sau
class PauseMenu : public IScreen
{
public:
    PauseMenu(const sf::Font& font, sf::Vector2u windowSize);

    PauseMenuAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    // Vẽ lớp phủ mờ + 3 nút lựa chọn
    void draw(sf::RenderWindow& window) override;

private:
    void updateHover(sf::Vector2f mousePos);

    sf::RectangleShape m_overlay; // Nền đen mờ phủ toàn màn hình

    sf::Text m_titleText; // Chữ "TAM DUNG"

    sf::RectangleShape m_resumeButton;
    sf::Text m_resumeText;

    sf::RectangleShape m_restartButton;
    sf::Text m_restartText;

    sf::RectangleShape m_mainMenuButton;
    sf::Text m_mainMenuText;
};