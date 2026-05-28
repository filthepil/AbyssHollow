#pragma once

#include "Core/Settings.h"

#include <string>

namespace abyss {

// Save/load boundary for future persistence. Keeping the signatures here makes
// it clear where world, floor, player, and settings serialization belongs.
class SaveSystem {
public:
    explicit SaveSystem(std::string saveDirectory = "saves");

    bool saveWorld() const;
    bool loadWorld();
    bool savePlayerState() const;
    bool loadPlayerState();
    bool saveSettings(const Settings& settings) const;

private:
    std::string m_saveDirectory;
};

} // namespace abyss
