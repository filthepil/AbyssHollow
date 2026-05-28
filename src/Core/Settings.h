#pragma once

#include <cstdint>
#include <string>

namespace abyss {

enum class DisplayMode {
    Windowed,
    BorderlessFullscreen,
    Fullscreen
};

struct Settings {
    unsigned int resolutionWidth = 1280;
    unsigned int resolutionHeight = 720;
    DisplayMode displayMode = DisplayMode::Windowed;
    float mainVolume = 80.f;
    float playerVolume = 80.f;
    std::string fontPath = "C:/Windows/Fonts/segoeui.ttf";
};

std::string toString(DisplayMode mode);
DisplayMode displayModeFromString(const std::string& value);
Settings loadSettingsFile(const std::string& path);
void saveSettingsFile(const std::string& path, const Settings& settings);

} // namespace abyss
