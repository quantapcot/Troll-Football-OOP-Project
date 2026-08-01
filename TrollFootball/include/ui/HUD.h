#pragma once
#include <SFML/Graphics.hpp>
#include "ui/ScoreBoard.h"
#include "ui/Timer.h"

// HUD (Heads-Up Display) là lớp giao diện luôn hiển thị trong lúc trận đấu diễn ra
// Nó KHÔNG tự vẽ chi tiết tỉ số/thời gian, mà "sở hữu" (composition) ScoreBoard và Timer
// rồi gọi lại các hàm draw() của 2 class đó => đúng nguyên tắc mỗi class chỉ lo 1 việc (SRP)
class HUD
{
public:
    // Nhận vào font chữ và tên 2 đội để khởi tạo ScoreBoard + Timer bên trong
    HUD(const sf::Font& font, const std::string& teamAName, const std::string& teamBName, float matchSeconds = 90.f);

    // Cập nhật mỗi frame: chủ yếu là cập nhật Timer đếm ngược
    void update(float deltaTime);

    // Vẽ toàn bộ HUD (tỉ số + đồng hồ) lên màn hình
    void draw(sf::RenderWindow& window);

    // Cho phép class bên ngoài (ví dụ GameplayScene) truy cập ScoreBoard/Timer
    // để cộng điểm khi có bàn thắng, hoặc pause khi mở PauseMenu
    ScoreBoard& getScoreBoard();
    Timer& getTimer();

private:
    ScoreBoard m_scoreBoard; // HUD "có một" ScoreBoard (quan hệ composition, không phải kế thừa)
    Timer m_timer;           // HUD "có một" Timer
};