#pragma once
#include <SFML/Graphics.hpp>

// Class Timer chịu trách nhiệm hiển thị thời gian trận đấu (dạng 01:30)
// và đếm ngược dần về 0
class Timer
{
public:
    // Constructor: khởi tạo timer với tổng thời gian trận đấu (đơn vị: giây)
    // font: tham chiếu tới font chữ dùng để vẽ (nên là digital.ttf cho giống đồng hồ điện tử)
    Timer(const sf::Font& font, float totalSeconds = 90.f);

    // update() được gọi mỗi frame trong vòng lặp game (game loop)
    // deltaTime: thời gian trôi qua giữa 2 frame (tính bằng giây), lấy từ sf::Clock::restart()
    void update(float deltaTime);

    // Vẽ đồng hồ lên màn hình
    void draw(sf::RenderWindow& window);

    // Tạm dừng / tiếp tục đếm giờ (dùng khi mở PauseMenu)
    void pause();
    void resume();

    // Kiểm tra xem thời gian đã hết chưa (đồng nghĩa hết trận)
    bool isTimeUp() const;

    // Reset lại đồng hồ về thời gian ban đầu (dùng khi bắt đầu trận mới)
    void reset();

private:
    // Cập nhật lại nội dung chữ hiển thị dựa trên m_timeLeft hiện tại
    void updateDisplayText();

    float m_totalTime;   // Tổng thời gian trận đấu (giây), dùng để reset()
    float m_timeLeft;    // Thời gian còn lại (giây)
    bool  m_isPaused;    // true => không đếm giờ (game đang tạm dừng)

    sf::Text m_text;     // Đối tượng chữ SFML dùng để vẽ thời gian lên màn hình
};