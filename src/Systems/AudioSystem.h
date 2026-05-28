#pragma once

namespace abyss {

// Placeholder audio layer. Hook setting volumes into SFML sound/music here when
// ambience, creature sound emitters, and player echolocation pings are added.
class AudioSystem {
public:
    void setMainVolume(float volume);
    void setPlayerVolume(float volume);
    float mainVolume() const;
    float playerVolume() const;

private:
    float m_mainVolume = 80.f;
    float m_playerVolume = 80.f;
};

} // namespace abyss
