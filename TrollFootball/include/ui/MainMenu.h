#pragma once
#include <SFML/Graphics.hpp>

// Enum liệt kê các lựa chọn người chơi có thể bấm trên MainMenu
// Dùng enum class thay vì enum thường để tránh xung đột tên với các enum khác trong project
enum class MainMenuAction
{
    None,   // Chưa bấm gì cả
    Play,   // Bấm nút "Chơi ngay"
    Exit    // Bấm nút "Thoát"
};

class MainMenu
{
public:
    MainMenu(const sf::Font& font, sf::Vector2u windowSize);

    // Xử lý sự kiện chuột (di chuyển để đổi màu nút khi hover, click để chọn)
    // event: sự kiện SFML lấy được từ window.pollEvent(event) trong vòng lặp game
    // Trả về hành động tương ứng nếu người chơi vừa click 1 nút, ngược lại trả về None
    MainMenuAction handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    // Vẽ toàn bộ MainMenu: tiêu đề game + 2 nút bấm
    void draw(sf::RenderWindow& window);

private:
    // Đổi màu nút khi con trỏ chuột đang ở trong vùng nút (hiệu ứng hover)
    void updateHover(sf::Vector2f mousePos);

    sf::Text m_title;         // Tiêu đề "HEAD FOOTBALL"

    sf::RectangleShape m_playButton; // Hình chữ nhật làm nền cho nút Play
    sf::Text m_playText;             // Chữ "CHOI NGAY" nằm trên nút

    sf::RectangleShape m_exitButton; // Hình chữ nhật làm nền cho nút Exit
    sf::Text m_exitText;             // Chữ "THOAT" nằm trên nút
};