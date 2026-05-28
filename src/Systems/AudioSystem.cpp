#include "Systems/AudioSystem.h"

#include <algorithm>

namespace abyss {

void AudioSystem::setMainVolume(float volume) {
    m_mainVolume = std::clamp(volume, 0.f, 100.f);
}

void AudioSystem::setPlayerVolume(float volume) {
    m_playerVolume = std::clamp(volume, 0.f, 100.f);
}

float AudioSystem::mainVolume() const {
    return m_mainVolume;
}

float AudioSystem::playerVolume() const {
    return m_playerVolume;
}

} // namespace abyss
