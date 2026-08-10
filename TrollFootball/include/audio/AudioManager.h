#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

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

    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;

    std::unordered_map<std::string, sf::Sound> m_sounds;

    sf::Music m_music;

    std::unordered_map<std::string, std::string> m_musicPaths;
};