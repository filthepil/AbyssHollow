#include "Systems/SaveSystem.h"

#include "Core/Settings.h"

#include <utility>

namespace abyss {

SaveSystem::SaveSystem(std::string saveDirectory)
    : m_saveDirectory(std::move(saveDirectory)) {}

bool SaveSystem::saveWorld() const {
    // TODO: Serialize generated floors, discovered map knowledge, and creature placement.
    return false;
}

bool SaveSystem::loadWorld() {
    // TODO: Rebuild the world and active floor from save data.
    return false;
}

bool SaveSystem::savePlayerState() const {
    // TODO: Persist player location, health/heart state, inventory, and progression.
    return false;
}

bool SaveSystem::loadPlayerState() {
    // TODO: Restore player state before the gameplay state starts running.
    return false;
}

bool SaveSystem::saveSettings(const Settings& settings) const {
    saveSettingsFile(m_saveDirectory + "/settings.ini", settings);
    return true;
}

} // namespace abyss
