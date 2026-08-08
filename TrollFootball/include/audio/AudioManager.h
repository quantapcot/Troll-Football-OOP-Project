#pragma once
#include <SFML/Audio.hpp>   // Thu vien am thanh cua SFML (sf::Sound, sf::Music, sf::SoundBuffer)
#include <unordered_map>    // Dung de luu danh sach am thanh theo ten (key - value)
#include <string>

// AudioManager duoc thiet ke theo mau Singleton
// => Chi co DUY NHAT 1 doi tuong AudioManager ton tai trong suot game
// => Bat ky file UI nao (HUD, MainMenu, PauseMenu...) cung goi duoc AudioManager::getInstance()
//    ma khong can truyen doi tuong qua lai giua cac class => tranh code roi
class AudioManager
{
public:
    static AudioManager& getInstance()
    {
        static AudioManager instance;
        return instance;
    }

    bool loadAll();

    void playSound(const std::string& soundName);

    void playMusic(const std::string& musicName, bool loop = true);

    void stopMusic();

    void setSoundVolume(float volume);

    void setMusicVolume(float volume);

    bool isMuted() const { return m_isMuted; }
    void setMuted(bool mute);

    float getMasterVolume() const { return m_masterVolume; }
    void setMasterVolume(float volume);

private:
    bool m_isMuted{ false };
    float m_masterVolume{ 100.f };
    AudioManager() = default;
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // Kho chua toan bo SoundBuffer (du lieu tho cua am thanh) theo ten
    // QUAN TRONG: cac sf::Sound ben duoi se GIU THAM CHIEU toi cac buffer nay,
    // nen unordered_map nay phai "song" suot vong doi AudioManager, khong duoc xoa buffer
    // trong khi con Sound nao dang tham chieu toi no.
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;

    // ĐIỂM KHÁC BIỆT LỚN NHẤT SO VỚI SFML 2.x:
    // Trong SFML 3, sf::Sound KHÔNG CÒN constructor mặc định - bắt buộc phải khởi tạo
    // với 1 sf::SoundBuffer ngay từ đầu: sf::Sound s(buffer);
    // => Không thể dùng 1 biến sf::Sound duy nhất dùng chung như bản 2.x nữa.
    // => Giải pháp: tạo riêng 1 sf::Sound cho MỖI hiệu ứng âm thanh, lưu theo tên.
    std::unordered_map<std::string, sf::Sound> m_sounds;

    // sf::Music vẫn còn constructor mặc định trong SFML 3 (vì nó "mở" file để stream
    // chứ không cần dữ liệu ngay lúc tạo đối tượng), nên giữ nguyên cách khai báo này.
    sf::Music m_music;

    std::unordered_map<std::string, std::string> m_musicPaths;
};