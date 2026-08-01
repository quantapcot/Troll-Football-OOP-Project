#include "ui/HUD.h"

HUD::HUD(const sf::Font& font, const std::string& teamAName, const std::string& teamBName, float matchSeconds)
// Khởi tạo trực tiếp 2 thành viên con thông qua member initializer list
// => tránh việc phải gọi constructor mặc định rồi gán lại (tối ưu hơn)
    : m_scoreBoard(font, teamAName, teamBName)
    , m_timer(font, matchSeconds)
{
}

void HUD::update(float deltaTime)
{
    // HUD chỉ cần "chuyển tiếp" lệnh update xuống cho Timer
    // ScoreBoard không cần update() mỗi frame vì nó chỉ đổi khi có bàn thắng (event-based)
    m_timer.update(deltaTime);
}

void HUD::draw(sf::RenderWindow& window)
{
    m_scoreBoard.draw(window); // Vẽ tỉ số trước (góc trên trái)
    m_timer.draw(window);      // Vẽ đồng hồ sau (giữa màn hình)
}

ScoreBoard& HUD::getScoreBoard()
{
    return m_scoreBoard; // Trả về tham chiếu để bên ngoài gọi addScoreA()/addScoreB()
}

Timer& HUD::getTimer()
{
    return m_timer; // Trả về tham chiếu để bên ngoài gọi pause()/resume()
}