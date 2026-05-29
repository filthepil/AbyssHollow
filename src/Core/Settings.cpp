#include "Core/Settings.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

namespace abyss {
namespace {

std::string trim(std::string value) {
    auto notSpace = [](unsigned char ch) { return !std::isspace(ch); };
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), notSpace));
    value.erase(std::find_if(value.rbegin(), value.rend(), notSpace).base(), value.end());
    return value;
}

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

float clampVolume(float value) {
    return std::clamp(value, 0.f, 100.f);
}

bool parseBool(const std::string& value) {
    const std::string normalized = lower(trim(value));
    return normalized == "1" || normalized == "true" || normalized == "yes" || normalized == "on";
}

float clampUiScale(float value) {
    return std::clamp(value, 0.5f, 2.f);
}

float comfortableFitScale(unsigned int width, unsigned int height) {
    constexpr float baseUiWidth = 900.f;
    constexpr float baseUiHeight = 900.f;
    constexpr float safeScreenFill = 0.92f;
    if (width == 0 || height == 0) {
        return 2.f;
    }

    return std::min((static_cast<float>(width) * safeScreenFill) / baseUiWidth,
                    (static_cast<float>(height) * safeScreenFill) / baseUiHeight);
}

} // namespace

std::string toString(DisplayMode mode) {
    switch (mode) {
        case DisplayMode::Fullscreen: return "fullscreen";
        case DisplayMode::BorderlessFullscreen: return "borderless";
        case DisplayMode::Windowed: return "windowed";
    }
    return "windowed";
}

std::string displayModeLabel(DisplayMode mode) {
    switch (mode) {
        case DisplayMode::Fullscreen: return "Exclusive Fullscreen";
        case DisplayMode::BorderlessFullscreen: return "Borderless Fullscreen";
        case DisplayMode::Windowed: return "Windowed";
    }
    return "Windowed";
}

DisplayMode displayModeFromString(const std::string& value) {
    const std::string normalized = lower(trim(value));
    if (normalized == "fullscreen") {
        return DisplayMode::Fullscreen;
    }
    if (normalized == "borderless" || normalized == "borderless_fullscreen" || normalized == "borderless windowed fullscreen") {
        return DisplayMode::BorderlessFullscreen;
    }
    return DisplayMode::Windowed;
}

float automaticUiScale(unsigned int width, unsigned int height) {
    constexpr float referenceWidth = 2560.f;
    constexpr float referenceHeight = 1600.f;
    if (width == 0 || height == 0) {
        return 1.f;
    }

    const float widthScale = static_cast<float>(width) / referenceWidth;
    const float heightScale = static_cast<float>(height) / referenceHeight;
    return fitUiScaleToScreen(width, height, std::min(widthScale, heightScale) * 2.f);
}

float fitUiScaleToScreen(unsigned int width, unsigned int height, float requestedScale) {
    return std::clamp(requestedScale, 0.5f, std::max(0.5f, std::min(2.f, comfortableFitScale(width, height))));
}

Settings loadSettingsFile(const std::string& path) {
    Settings settings;
    std::ifstream input(path);
    if (!input) {
        return settings;
    }

    std::string line;
    while (std::getline(input, line)) {
        line = trim(line);
        if (line.empty() || line.front() == '#') {
            continue;
        }

        const auto equals = line.find('=');
        if (equals == std::string::npos) {
            continue;
        }

        const std::string key = lower(trim(line.substr(0, equals)));
        const std::string value = trim(line.substr(equals + 1));
        std::stringstream parser(value);

        if (key == "resolution_width") {
            parser >> settings.resolutionWidth;
        } else if (key == "resolution_height") {
            parser >> settings.resolutionHeight;
        } else if (key == "display_mode") {
            settings.displayMode = displayModeFromString(value);
        } else if (key == "ui_scale") {
            parser >> settings.uiScale;
            settings.uiScale = clampUiScale(settings.uiScale);
        } else if (key == "ui_scale_manual") {
            settings.uiScaleManual = parseBool(value);
        } else if (key == "main_volume") {
            parser >> settings.mainVolume;
            settings.mainVolume = clampVolume(settings.mainVolume);
        } else if (key == "player_volume") {
            parser >> settings.playerVolume;
            settings.playerVolume = clampVolume(settings.playerVolume);
        } else if (key == "font_path") {
            settings.fontPath = value;
        }
    }

    return settings;
}

void saveSettingsFile(const std::string& path, const Settings& settings) {
    std::ofstream output(path);
    output << "# Abyss Hollow settings are intentionally plain text so designers can edit defaults.\n";
    output << "resolution_width=" << settings.resolutionWidth << '\n';
    output << "resolution_height=" << settings.resolutionHeight << '\n';
    output << "display_mode=" << toString(settings.displayMode) << '\n';
    output << "ui_scale=" << settings.uiScale << '\n';
    output << "ui_scale_manual=" << (settings.uiScaleManual ? "true" : "false") << '\n';
    output << "main_volume=" << settings.mainVolume << '\n';
    output << "player_volume=" << settings.playerVolume << '\n';
    output << "font_path=" << settings.fontPath << '\n';
}

} // namespace abyss
