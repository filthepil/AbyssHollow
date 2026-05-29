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
    unsigned int resolutionWidth = 0;
    unsigned int resolutionHeight = 0;
    DisplayMode displayMode = DisplayMode::BorderlessFullscreen;
    float uiScale = 1.f;
    bool uiScaleManual = false;
    float mainVolume = 80.f;
    float playerVolume = 80.f;
    std::string fontPath = "assets/fonts/main.ttf";
};

std::string toString(DisplayMode mode);
std::string displayModeLabel(DisplayMode mode);
DisplayMode displayModeFromString(const std::string& value);
float automaticUiScale(unsigned int width, unsigned int height);
Settings loadSettingsFile(const std::string& path);
void saveSettingsFile(const std::string& path, const Settings& settings);

} // namespace abyss
