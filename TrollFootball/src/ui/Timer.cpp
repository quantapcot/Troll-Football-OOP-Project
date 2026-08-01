#include "ui/Timer.h"
#include "audio/AudioManager.h"
#include <sstream>
#include <iomanip>

Timer::Timer(const sf::Font& font, float totalSeconds)
    : m_totalTime(totalSeconds)
    , m_timeLeft(totalSeconds)
    , m_isPaused(false)
    // SFML 3: sf::Text KHÔNG còn constructor mặc định, bắt buộc truyền Font ngay ở đây.
    // Nếu không có dòng này, code sẽ báo lỗi biên dịch vì trình biên dịch không biết
    // khởi tạo m_text (khai báo trong Timer.h) bằng cách nào.
    , m_text(font)
{
    m_text.setCharacterSize(40);
    m_text.setFillColor(sf::Color::White);

    // setPosition ở SFML 3 CHỈ nhận 1 tham số kiểu sf::Vector2f, không còn overload (x, y) nữa
    // => truyền dạng brace-init {x, y} để trình biên dịch tự tạo Vector2f
    m_text.setPosition({ 460.f, 20.f });

    updateDisplayText();
}

void Timer::update(float deltaTime)
{
    if (m_isPaused || m_timeLeft <= 0.f)
        return;

    m_timeLeft -= deltaTime;
    if (m_timeLeft < 0.f)
        m_timeLeft = 0.f;

    if (m_timeLeft <= 3.f && m_timeLeft > 0.f)
    {
        AudioManager::getInstance().playSound("countdown");
    }

    updateDisplayText();
}

void Timer::updateDisplayText()
{
    int totalSecondsLeft = static_cast<int>(m_timeLeft);
    int minutes = totalSecondsLeft / 60;
    int seconds = totalSecondsLeft % 60;

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << minutes
        << ":"
        << std::setfill('0') << std::setw(2) << seconds;

    m_text.setString(oss.str());
}

void Timer::draw(sf::RenderWindow& window)
{
    window.draw(m_text);
}

void Timer::pause() { m_isPaused = true; }
void Timer::resume() { m_isPaused = false; }
bool Timer::isTimeUp() const { return m_timeLeft <= 0.f; }

void Timer::reset()
{
    m_timeLeft = m_totalTime;
    m_isPaused = false;
    updateDisplayText();
}