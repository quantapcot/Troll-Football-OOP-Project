#include "audio/AudioManager.h"
#include <iostream>

bool AudioManager::loadAll()
{
    bool allOk = true;

    std::unordered_map<std::string, std::string> soundFiles = {
        {"kick",      "assets/sounds/kick.wav"},
        {"jump",      "assets/sounds/jump.wav"},
        {"goal",      "assets/sounds/goal.wav"},
        {"whistle",   "assets/sounds/whistle.wav"},
        {"button",    "assets/sounds/button.wav"},
        {"countdown", "assets/sounds/countdown.wav"},
        {"asteroid",  "assets/sounds/asteroid.wav"},
        {"stun",      "assets/sounds/stun.wav"}
    };
	//FIND THE SOUND FILES IN THE ASSETS FOLDER
    for (auto& pair : soundFiles)
    {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(pair.second))
        {
            std::cerr << "[AudioManager] Can't load file: " << pair.second << std::endl;
            allOk = false;
            continue;
        }

        auto insertedBuffer = m_soundBuffers.emplace(pair.first, std::move(buffer));

        m_sounds.emplace(pair.first, sf::Sound(insertedBuffer.first->second));
    }

    m_musicPaths = {
        {"menu",    "assets/music/menu.mp3"},
        {"match",   "assets/music/match.mp3"},
        {"victory", "assets/music/victory.mp3"}
    };

    return allOk;
}

void AudioManager::playSound(const std::string& soundName)
{
    auto it = m_sounds.find(soundName);
    if (it == m_sounds.end())
    {
        std::cerr << "[AudioManager] Khong tim thay sound: " << soundName << std::endl;
        return;
    }

    it->second.play();
}

void AudioManager::playMusic(const std::string& musicName, bool loop)
{
    auto it = m_musicPaths.find(musicName);
    if (it == m_musicPaths.end())
    {
        std::cerr << "[AudioManager] Khong tim thay music: " << musicName << std::endl;
        return;
    }

    if (!m_music.openFromFile(it->second))
    {
        std::cerr << "[AudioManager] Loi mo file nhac: " << it->second << std::endl;
        return;
    }

    m_music.setLooping(loop);
    m_music.play();
}

void AudioManager::stopMusic()
{
    m_music.stop();
}

void AudioManager::setSoundVolume(float volume)
{
    for (auto& pair : m_sounds)
        pair.second.setVolume(volume);
}

void AudioManager::setMusicVolume(float volume)
{
    m_music.setVolume(volume);
}

void AudioManager::setMuted(bool mute)
{
    m_isMuted = mute;
    if (m_isMuted)
    {
        setSoundVolume(0.f);
        setMusicVolume(0.f);
    }
    else
    {
        setSoundVolume(m_masterVolume);
        setMusicVolume(m_masterVolume);
    }
}

void AudioManager::setMasterVolume(float volume)
{
    if (volume < 0.f) volume = 0.f;
    if (volume > 100.f) volume = 100.f;
    m_masterVolume = volume;
    if (!m_isMuted)
    {
        setSoundVolume(m_masterVolume);
        setMusicVolume(m_masterVolume);
    }
}