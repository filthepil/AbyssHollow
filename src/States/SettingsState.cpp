#include "States/SettingsState.h"

#include "Core/Game.h"

#include <SFML/Window/VideoMode.hpp>

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <string>

namespace abyss {
namespace {

constexpr unsigned int MinimumWidth = 800;
constexpr unsigned int MinimumHeight = 600;

bool isDigits(const std::string& value) {
    return !value.empty() && std::all_of(value.begin(), value.end(), [](unsigned char ch) {
        return std::isdigit(ch) != 0;
    });
}

unsigned int textSize(float baseSize, float scale) {
    return static_cast<unsigned int>(std::round(baseSize * scale));
}

} // namespace

SettingsState::SettingsState(Game& game)
    : State(game),
      m_displayMode(game.settings().displayMode),
      m_scale(game.uiScale()),
      m_uiScaleSlider(game.resources().font("default"), "UI Size", 50.f, 200.f, game.settings().uiScale * 100.f, m_scale),
      m_mainVolume(game.resources().font("default"), "Main volume", 0.f, 100.f, game.settings().mainVolume, m_scale),
      m_playerVolume(game.resources().font("default"), "Player volume", 0.f, 100.f, game.settings().playerVolume, m_scale) {
    const auto& font = m_game.resources().font("default");

    const std::array<sf::Vector2u, 6> commonResolutions{{
        {1600, 900},
        {1920, 1080},
        {2560, 1440},
        {2560, 1600},
        {3440, 1440},
        {3840, 2160},
    }};
    for (const auto& resolution : commonResolutions) {
        addResolution(resolution);
    }

    const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    addResolution({desktop.width, desktop.height});
    addResolution({m_game.settings().resolutionWidth, m_game.settings().resolutionHeight});
    std::sort(m_resolutions.begin(), m_resolutions.end(), [](sf::Vector2u left, sf::Vector2u right) {
        if (left.x == right.x) {
            return left.y < right.y;
        }
        return left.x < right.x;
    });

    const sf::Vector2u current{m_game.settings().resolutionWidth, m_game.settings().resolutionHeight};
    const auto found = std::find(m_resolutions.begin(), m_resolutions.end(), current);
    if (found != m_resolutions.end()) {
        m_resolutionIndex = static_cast<std::size_t>(std::distance(m_resolutions.begin(), found));
    }

    m_panel.setSize({scaled(760.f), scaled(820.f)});
    m_panel.setPosition({scaled(60.f), scaled(40.f)});
    m_panel.setFillColor(sf::Color(7, 7, 12, 245));
    m_panel.setOutlineColor(sf::Color(90, 90, 120));
    m_panel.setOutlineThickness(scaled(2.f));

    m_title.setFont(font);
    m_title.setString("Settings");
    m_title.setCharacterSize(textSize(42.f, m_scale));
    m_title.setFillColor(sf::Color::White);
    m_title.setPosition({scaled(95.f), scaled(65.f)});

    m_resolutionLabel.setFont(font);
    m_resolutionLabel.setString("Resolution");
    m_resolutionLabel.setCharacterSize(textSize(22.f, m_scale));
    m_resolutionLabel.setFillColor(sf::Color::White);
    m_resolutionLabel.setPosition({scaled(100.f), scaled(135.f)});

    m_resolutionBox.setSize({scaled(310.f), scaled(44.f)});
    m_resolutionBox.setPosition({scaled(335.f), scaled(128.f)});
    m_resolutionBox.setFillColor(sf::Color(18, 18, 24));
    m_resolutionBox.setOutlineColor(sf::Color(110, 110, 135));
    m_resolutionBox.setOutlineThickness(scaled(2.f));

    m_resolutionValue.setFont(font);
    m_resolutionValue.setCharacterSize(textSize(20.f, m_scale));
    m_resolutionValue.setFillColor(sf::Color::White);

    m_customLabel.setFont(font);
    m_customLabel.setString("Custom width / height");
    m_customLabel.setCharacterSize(textSize(20.f, m_scale));
    m_customLabel.setFillColor(sf::Color(190, 195, 215));
    m_customLabel.setPosition({scaled(100.f), scaled(195.f)});

    m_widthBox.setSize({scaled(120.f), scaled(40.f)});
    m_widthBox.setPosition({scaled(335.f), scaled(188.f)});
    m_widthBox.setFillColor(sf::Color(18, 18, 24));
    m_widthBox.setOutlineThickness(scaled(2.f));

    m_heightBox = m_widthBox;
    m_heightBox.setPosition({scaled(525.f), scaled(188.f)});

    m_widthText.setFont(font);
    m_widthText.setCharacterSize(textSize(19.f, m_scale));
    m_widthText.setFillColor(sf::Color::White);
    m_heightText = m_widthText;

    m_byText.setFont(font);
    m_byText.setString("x");
    m_byText.setCharacterSize(textSize(22.f, m_scale));
    m_byText.setFillColor(sf::Color::White);
    m_byText.setPosition({scaled(482.f), scaled(195.f)});

    m_displayModeLabel.setFont(font);
    m_displayModeLabel.setCharacterSize(textSize(22.f, m_scale));
    m_displayModeLabel.setFillColor(sf::Color::White);
    m_displayModeLabel.setPosition({scaled(100.f), scaled(265.f)});

    m_displayModeButton = std::make_unique<ui::Button>(font, "Change", sf::Vector2f{scaled(170.f), scaled(46.f)}, m_scale);
    m_displayModeButton->setPosition({scaled(475.f), scaled(255.f)});
    m_displayModeButton->setCallback([this] { cycleDisplayMode(); });

    m_uiScaleSlider.setPosition({scaled(100.f), scaled(340.f)});
    m_mainVolume.setPosition({scaled(100.f), scaled(430.f)});
    m_playerVolume.setPosition({scaled(100.f), scaled(520.f)});

    m_errorText.setFont(font);
    m_errorText.setCharacterSize(textSize(18.f, m_scale));
    m_errorText.setFillColor(sf::Color(255, 135, 135));
    m_errorText.setPosition({scaled(100.f), scaled(610.f)});

    m_applyButton = std::make_unique<ui::Button>(font, "Apply", sf::Vector2f{scaled(180.f), scaled(50.f)}, m_scale);
    m_applyButton->setPosition({scaled(100.f), scaled(690.f)});
    m_applyButton->setCallback([this] { applyAndClose(); });

    m_backButton = std::make_unique<ui::Button>(font, "Back", sf::Vector2f{scaled(180.f), scaled(50.f)}, m_scale);
    m_backButton->setPosition({scaled(310.f), scaled(690.f)});
    m_backButton->setCallback([this] { m_game.closeCurrentState(); });

    refreshResolutionInput();
    rebuildLabels();
}

void SettingsState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        m_game.closeCurrentState();
        return;
    }

    const float uiScaleBefore = m_uiScaleSlider.value();
    const auto mouse = sf::Vector2f(sf::Mouse::getPosition(m_game.window()));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (m_resolutionBox.getGlobalBounds().contains(mouse)) {
            m_resolutionDropdownOpen = !m_resolutionDropdownOpen;
            setInputFocus(FocusedInput::None);
        } else if (m_widthBox.getGlobalBounds().contains(mouse)) {
            setInputFocus(FocusedInput::Width);
            m_resolutionDropdownOpen = false;
        } else if (m_heightBox.getGlobalBounds().contains(mouse)) {
            setInputFocus(FocusedInput::Height);
            m_resolutionDropdownOpen = false;
        } else if (m_resolutionDropdownOpen) {
            const float optionHeight = scaled(36.f);
            const sf::Vector2f origin = m_resolutionBox.getPosition();
            for (std::size_t i = 0; i < m_resolutions.size(); ++i) {
                sf::FloatRect bounds(origin.x, origin.y + m_resolutionBox.getSize().y + static_cast<float>(i) * optionHeight,
                                     m_resolutionBox.getSize().x, optionHeight);
                if (bounds.contains(mouse)) {
                    m_resolutionIndex = i;
                    refreshResolutionInput();
                    rebuildLabels();
                    break;
                }
            }
            m_resolutionDropdownOpen = false;
        } else {
            setInputFocus(FocusedInput::None);
        }
    }

    handleTextInput(event);
    m_displayModeButton->handleEvent(event, m_game.window());
    m_uiScaleSlider.handleEvent(event, m_game.window());
    if (std::abs(m_uiScaleSlider.value() - uiScaleBefore) > 0.01f) {
        m_userChangedUiScale = true;
    }
    m_mainVolume.handleEvent(event, m_game.window());
    m_playerVolume.handleEvent(event, m_game.window());
    m_applyButton->handleEvent(event, m_game.window());
    m_backButton->handleEvent(event, m_game.window());
}

void SettingsState::update(float) {}

void SettingsState::draw(sf::RenderWindow& window) {
    window.draw(m_panel);
    window.draw(m_title);
    window.draw(m_resolutionLabel);
    window.draw(m_resolutionBox);
    window.draw(m_resolutionValue);
    window.draw(m_customLabel);
    window.draw(m_widthBox);
    window.draw(m_heightBox);
    window.draw(m_widthText);
    window.draw(m_heightText);
    window.draw(m_byText);
    window.draw(m_displayModeLabel);
    m_displayModeButton->draw(window);
    m_uiScaleSlider.draw(window);
    m_mainVolume.draw(window);
    m_playerVolume.draw(window);
    window.draw(m_errorText);
    m_applyButton->draw(window);
    m_backButton->draw(window);

    if (m_resolutionDropdownOpen) {
        const float optionHeight = scaled(36.f);
        const sf::Vector2f origin = m_resolutionBox.getPosition();
        for (std::size_t i = 0; i < m_resolutions.size(); ++i) {
            sf::RectangleShape option({m_resolutionBox.getSize().x, optionHeight});
            option.setPosition({origin.x, origin.y + m_resolutionBox.getSize().y + static_cast<float>(i) * optionHeight});
            option.setFillColor(i == m_resolutionIndex ? sf::Color(45, 50, 70) : sf::Color(14, 14, 20));
            option.setOutlineColor(sf::Color(80, 80, 105));
            option.setOutlineThickness(scaled(1.f));
            window.draw(option);

            sf::Text text;
            text.setFont(m_game.resources().font("default"));
            text.setString(resolutionText(m_resolutions[i]));
            text.setCharacterSize(textSize(18.f, m_scale));
            text.setFillColor(sf::Color::White);
            text.setPosition({option.getPosition().x + scaled(12.f), option.getPosition().y + scaled(6.f)});
            window.draw(text);
        }
    }
}

void SettingsState::addResolution(sf::Vector2u resolution) {
    if (resolution.x < MinimumWidth || resolution.y < MinimumHeight) {
        return;
    }
    if (std::find(m_resolutions.begin(), m_resolutions.end(), resolution) == m_resolutions.end()) {
        m_resolutions.push_back(resolution);
    }
}

void SettingsState::rebuildLabels() {
    m_resolutionValue.setString(resolutionText(m_resolutions[m_resolutionIndex]) + "  v");
    positionTextInBox(m_resolutionValue, m_resolutionBox);
    m_displayModeLabel.setString("Display mode: " + displayModeLabel(m_displayMode));
}

void SettingsState::cycleDisplayMode() {
    if (m_displayMode == DisplayMode::Windowed) {
        m_displayMode = DisplayMode::BorderlessFullscreen;
    } else if (m_displayMode == DisplayMode::BorderlessFullscreen) {
        m_displayMode = DisplayMode::Fullscreen;
    } else {
        m_displayMode = DisplayMode::Windowed;
    }
    rebuildLabels();
}

void SettingsState::applyAndClose() {
    sf::Vector2u resolution;
    if (!validateCustomResolution(resolution)) {
        return;
    }

    auto& settings = m_game.settings();
    const bool resolutionChanged = settings.resolutionWidth != resolution.x || settings.resolutionHeight != resolution.y;
    settings.resolutionWidth = resolution.x;
    settings.resolutionHeight = resolution.y;
    settings.displayMode = m_displayMode;
    settings.mainVolume = m_mainVolume.value();
    settings.playerVolume = m_playerVolume.value();

    if (m_userChangedUiScale) {
        settings.uiScale = std::clamp(m_uiScaleSlider.value() / 100.f, 0.5f, 2.f);
        settings.uiScaleManual = true;
    } else if (resolutionChanged && !settings.uiScaleManual) {
        settings.uiScale = automaticUiScale(resolution.x, resolution.y);
    } else {
        settings.uiScale = std::clamp(m_uiScaleSlider.value() / 100.f, 0.5f, 2.f);
    }

    saveSettingsFile("configs/settings.ini", settings);
    m_game.saves().saveSettings(settings);
    m_game.applyVideoSettings();
    m_game.closeCurrentState();
}

bool SettingsState::validateCustomResolution(sf::Vector2u& resolution) {
    if (!isDigits(m_widthInput) || !isDigits(m_heightInput)) {
        m_errorText.setString("Resolution must use numeric width and height values.");
        return false;
    }

    const unsigned long width = std::stoul(m_widthInput);
    const unsigned long height = std::stoul(m_heightInput);
    if (width < MinimumWidth || height < MinimumHeight) {
        m_errorText.setString("Resolution is too small. Minimum is 800 x 600.");
        return false;
    }
    if (width > 7680 || height > 4320) {
        m_errorText.setString("Resolution is too large for this settings menu.");
        return false;
    }

    resolution = {static_cast<unsigned int>(width), static_cast<unsigned int>(height)};
    addResolution(resolution);
    const auto found = std::find(m_resolutions.begin(), m_resolutions.end(), resolution);
    m_resolutionIndex = static_cast<std::size_t>(std::distance(m_resolutions.begin(), found));
    m_errorText.setString("");
    return true;
}

void SettingsState::refreshResolutionInput() {
    const auto resolution = m_resolutions[m_resolutionIndex];
    m_widthInput = std::to_string(resolution.x);
    m_heightInput = std::to_string(resolution.y);
    m_widthText.setString(m_widthInput);
    m_heightText.setString(m_heightInput);
    positionTextInBox(m_widthText, m_widthBox);
    positionTextInBox(m_heightText, m_heightBox);
}

void SettingsState::setInputFocus(FocusedInput input) {
    m_focusedInput = input;
    m_widthBox.setOutlineColor(input == FocusedInput::Width ? sf::Color(180, 200, 255) : sf::Color(110, 110, 135));
    m_heightBox.setOutlineColor(input == FocusedInput::Height ? sf::Color(180, 200, 255) : sf::Color(110, 110, 135));
}

void SettingsState::handleTextInput(const sf::Event& event) {
    if (m_focusedInput == FocusedInput::None || event.type != sf::Event::TextEntered) {
        return;
    }

    std::string& target = m_focusedInput == FocusedInput::Width ? m_widthInput : m_heightInput;
    if (event.text.unicode >= '0' && event.text.unicode <= '9') {
        if (target.size() < 5) {
            target.push_back(static_cast<char>(event.text.unicode));
        }
    } else if (event.text.unicode == 8 && !target.empty()) {
        target.pop_back();
    }

    if (m_focusedInput == FocusedInput::Width) {
        m_widthText.setString(m_widthInput);
        positionTextInBox(m_widthText, m_widthBox);
    } else {
        m_heightText.setString(m_heightInput);
        positionTextInBox(m_heightText, m_heightBox);
    }
}

void SettingsState::positionTextInBox(sf::Text& text, const sf::RectangleShape& box) {
    const sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition(
        box.getPosition().x + scaled(12.f) - bounds.left,
        box.getPosition().y + (box.getSize().y - bounds.height) / 2.f - bounds.top - scaled(2.f));
}

std::string SettingsState::resolutionText(sf::Vector2u resolution) const {
    return std::to_string(resolution.x) + " x " + std::to_string(resolution.y);
}

float SettingsState::scaled(float value) const {
    return value * m_scale;
}

} // namespace abyss
