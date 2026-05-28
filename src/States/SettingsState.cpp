#include "States/SettingsState.h"

#include "Core/Game.h"

#include <algorithm>
#include <array>

namespace abyss {

SettingsState::SettingsState(Game& game)
    : State(game),
      m_resolutions({{1280, 720}, {1600, 900}, {1920, 1080}}),
      m_displayMode(game.settings().displayMode),
      m_mainVolume(game.resources().font("default"), "Main volume", 0.f, 100.f, game.settings().mainVolume),
      m_playerVolume(game.resources().font("default"), "Player volume", 0.f, 100.f, game.settings().playerVolume) {
    const auto& font = m_game.resources().font("default");

    const sf::Vector2u current{m_game.settings().resolutionWidth, m_game.settings().resolutionHeight};
    auto found = std::find(m_resolutions.begin(), m_resolutions.end(), current);
    if (found == m_resolutions.end()) {
        m_resolutions.push_back(current);
        m_resolutionIndex = m_resolutions.size() - 1;
    } else {
        m_resolutionIndex = static_cast<std::size_t>(std::distance(m_resolutions.begin(), found));
    }

    m_panel.setSize({620.f, 620.f});
    m_panel.setPosition({60.f, 50.f});
    m_panel.setFillColor(sf::Color(7, 7, 12, 245));
    m_panel.setOutlineColor(sf::Color(90, 90, 120));
    m_panel.setOutlineThickness(2.f);

    m_title.setFont(font);
    m_title.setString("Settings");
    m_title.setCharacterSize(42);
    m_title.setFillColor(sf::Color::White);
    m_title.setPosition({95.f, 80.f});

    m_resolutionLabel.setFont(font);
    m_resolutionLabel.setCharacterSize(22);
    m_resolutionLabel.setFillColor(sf::Color::White);
    m_resolutionLabel.setPosition({100.f, 160.f});

    m_displayModeLabel.setFont(font);
    m_displayModeLabel.setCharacterSize(22);
    m_displayModeLabel.setFillColor(sf::Color::White);
    m_displayModeLabel.setPosition({100.f, 240.f});

    m_resolutionButton = std::make_unique<ui::Button>(font, "Change", sf::Vector2f{150.f, 46.f});
    m_resolutionButton->setPosition({500.f, 150.f});
    m_resolutionButton->setCallback([this] { cycleResolution(); });

    m_displayModeButton = std::make_unique<ui::Button>(font, "Change", sf::Vector2f{150.f, 46.f});
    m_displayModeButton->setPosition({500.f, 230.f});
    m_displayModeButton->setCallback([this] { cycleDisplayMode(); });

    m_mainVolume.setPosition({100.f, 320.f});
    m_playerVolume.setPosition({100.f, 400.f});

    m_applyButton = std::make_unique<ui::Button>(font, "Apply", sf::Vector2f{180.f, 50.f});
    m_applyButton->setPosition({100.f, 540.f});
    m_applyButton->setCallback([this] { applyAndClose(); });

    m_backButton = std::make_unique<ui::Button>(font, "Back", sf::Vector2f{180.f, 50.f});
    m_backButton->setPosition({300.f, 540.f});
    m_backButton->setCallback([this] { m_game.closeCurrentState(); });

    rebuildLabels();
}

void SettingsState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        m_game.closeCurrentState();
        return;
    }
    m_resolutionButton->handleEvent(event, m_game.window());
    m_displayModeButton->handleEvent(event, m_game.window());
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
    window.draw(m_displayModeLabel);
    m_resolutionButton->draw(window);
    m_displayModeButton->draw(window);
    m_mainVolume.draw(window);
    m_playerVolume.draw(window);
    m_applyButton->draw(window);
    m_backButton->draw(window);
}

void SettingsState::rebuildLabels() {
    const auto resolution = m_resolutions[m_resolutionIndex];
    m_resolutionLabel.setString("Resolution: " + std::to_string(resolution.x) + " x " + std::to_string(resolution.y));
    m_displayModeLabel.setString("Display mode: " + toString(m_displayMode));
}

void SettingsState::cycleResolution() {
    m_resolutionIndex = (m_resolutionIndex + 1) % m_resolutions.size();
    rebuildLabels();
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
    auto& settings = m_game.settings();
    const auto resolution = m_resolutions[m_resolutionIndex];
    settings.resolutionWidth = resolution.x;
    settings.resolutionHeight = resolution.y;
    settings.displayMode = m_displayMode;
    settings.mainVolume = m_mainVolume.value();
    settings.playerVolume = m_playerVolume.value();

    // Settings are saved separately from gameplay data so future save slots can
    // coexist with global video/audio preferences.
    saveSettingsFile("configs/settings.ini", settings);
    m_game.saves().saveSettings(settings);
    m_game.applyVideoSettings();
    m_game.closeCurrentState();
}

} // namespace abyss
