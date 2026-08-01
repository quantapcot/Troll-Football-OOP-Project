#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Class ScoreBoard hiển thị tỉ số của 2 đội, dạng "Team A  2 - 1  Team B"
class ScoreBoard
{
public:
    // Constructor nhận tên 2 đội và font chữ để vẽ
    ScoreBoard(const sf::Font& font, const std::string& teamAName, const std::string& teamBName);

    // Cộng thêm 1 bàn thắng cho đội A hoặc đội B
    void addScoreA();
    void addScoreB();

    // Lấy điểm hiện tại (dùng để so sánh xác định thắng/thua khi kết thúc trận)
    int getScoreA() const;
    int getScoreB() const;

    // Vẽ bảng tỉ số lên màn hình, gọi mỗi frame trong game loop
    void draw(sf::RenderWindow& window);

    // Reset tỉ số về 0-0 (dùng khi bắt đầu trận mới)
    void reset();

private:
    // Ghép lại chuỗi hiển thị "TenDoiA  ScoreA - ScoreB  TenDoiB" và gán vào m_text
    void updateDisplayText();

    std::string m_teamAName;
    std::string m_teamBName;

    int m_scoreA; // Số bàn thắng đội A
    int m_scoreB; // Số bàn thắng đội B

    sf::Text m_text; // Đối tượng chữ dùng để vẽ toàn bộ dòng tỉ số
};