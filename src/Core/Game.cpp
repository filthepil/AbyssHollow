#include "Core/Game.h"

#include "States/GameplayState.h"
#include "States/MainMenuState.h"
#include "States/PauseMenuState.h"
#include "States/SettingsState.h"

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <memory>
#include <string>

namespace abyss {

Game::Game()
    : m_settings(loadSettingsFile("configs/settings.ini")),
      m_saves("saves") {
    initializeDisplaySettings();
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
            } else if (event.type == sf::Event::Resized) {
                m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width),
                                                        static_cast<float>(event.size.height))));
                m_states.onDisplayChanged();
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
float Game::uiScale() const { return m_settings.uiScale; }

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
    m_states.onDisplayChanged();
}

void Game::initializeDisplaySettings() {
    const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    if (m_settings.resolutionWidth == 0 || m_settings.resolutionHeight == 0) {
        m_settings.resolutionWidth = desktop.width;
        m_settings.resolutionHeight = desktop.height;
    }

    if (!m_settings.uiScaleManual) {
        m_settings.uiScale = automaticUiScale(m_settings.resolutionWidth, m_settings.resolutionHeight);
    } else {
        m_settings.uiScale = fitUiScaleToScreen(m_settings.resolutionWidth, m_settings.resolutionHeight, m_settings.uiScale);
    }
}

void Game::createWindow() {
    sf::VideoMode mode(m_settings.resolutionWidth, m_settings.resolutionHeight);
    sf::Uint32 style = sf::Style::Titlebar | sf::Style::Close;

    if (m_settings.displayMode == DisplayMode::Fullscreen) {
        style = sf::Style::Fullscreen;
    } else if (m_settings.displayMode == DisplayMode::BorderlessFullscreen) {
        mode = sf::VideoMode(m_settings.resolutionWidth, m_settings.resolutionHeight);
        style = sf::Style::None;
    }

    m_window.create(mode, "Abyss Hollow", style);
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(mode.width), static_cast<float>(mode.height))));
    m_window.setFramerateLimit(60);
}

void Game::loadCoreAssets() {
    if (m_resources.loadFont("default", m_settings.fontPath)) {
        return;
    }

    std::cerr << "Failed to load the default UI font at " << m_settings.fontPath
              << ". Add assets/fonts/main.ttf or update font_path in configs/settings.ini.\n";
}

} // namespace abyss
