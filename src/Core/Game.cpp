#include "Core/Game.h"

#include "States/GameplayState.h"
#include "States/MainMenuState.h"
#include "States/PauseMenuState.h"
#include "States/SettingsState.h"

#include <SFML/System/Clock.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace abyss {

Game::Game()
    : m_settings(loadSettingsFile("configs/settings.ini")),
      m_saves("saves") {
    createWindow();
    loadCoreAssets();
    showMainMenu();
}

void Game::run() {
    sf::Clock clock;
    while (m_running && m_window.isOpen()) {
        const float deltaSeconds = clock.restart().asSeconds();

        sf::Event event{};
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                quitToDesktop();
            }
            if (State* state = m_states.top()) {
                state->handleEvent(event);
            }
        }

        if (State* state = m_states.top()) {
            state->update(deltaSeconds);
        }

        m_window.clear(sf::Color::Black);
        if (State* state = m_states.top()) {
            state->draw(m_window);
        }
        m_window.display();
    }
}

sf::RenderWindow& Game::window() { return m_window; }
ResourceManager& Game::resources() { return m_resources; }
AudioSystem& Game::audio() { return m_audio; }
SaveSystem& Game::saves() { return m_saves; }
Settings& Game::settings() { return m_settings; }

void Game::showMainMenu() {
    m_states.clear();
    m_states.push(std::make_unique<MainMenuState>(*this));
}

void Game::startNewGame() {
    // TODO: Initialize save slot, generated floor data, and restored player state here.
    m_states.clear();
    m_states.push(std::make_unique<GameplayState>(*this));
}

void Game::showPauseMenu() {
    m_states.push(std::make_unique<PauseMenuState>(*this));
}

void Game::showSettings() {
    m_states.push(std::make_unique<SettingsState>(*this));
}

void Game::closeCurrentState() {
    m_states.pop();
    if (m_states.empty()) {
        showMainMenu();
    }
}

void Game::quitToDesktop() {
    m_running = false;
    m_window.close();
}

void Game::applyVideoSettings() {
    createWindow();
}

void Game::createWindow() {
    sf::VideoMode mode(m_settings.resolutionWidth, m_settings.resolutionHeight);
    sf::Uint32 style = sf::Style::Titlebar | sf::Style::Close;

    if (m_settings.displayMode == DisplayMode::Fullscreen) {
        style = sf::Style::Fullscreen;
    } else if (m_settings.displayMode == DisplayMode::BorderlessFullscreen) {
        mode = sf::VideoMode::getDesktopMode();
        style = sf::Style::None;
    }

    m_window.create(mode, "Abyss Hollow", style);
    m_window.setFramerateLimit(60);
}

void Game::loadCoreAssets() {
    // Prefer Segoe UI, a standard Windows UI font, so the project no longer
    // ships a font file in the archive. The additional candidates are also
    // standard Windows fonts and are not copied into the game.
    const std::vector<std::string> fontCandidates{
        m_settings.fontPath,
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/tahoma.ttf"
    };

    for (const auto& path : fontCandidates) {
        if (m_resources.loadFont("default", path)) {
            m_settings.fontPath = path;
            return;
        }
    }

    std::cerr << "Failed to load a default UI font. Set font_path in configs/settings.ini.\n";
}

} // namespace abyss
