#include "States/MainMenuState.h"

#include "Core/Game.h"

namespace abyss {

MainMenuState::MainMenuState(Game& game)
    : State(game) {
    const auto& font = m_game.resources().font("default");
    const float scale = m_game.uiScale();

    m_title.setFont(font);
    m_title.setString("ABYSS HOLLOW");
    m_title.setCharacterSize(static_cast<unsigned int>(58.f * scale));
    m_title.setFillColor(sf::Color(225, 230, 255));
    m_title.setPosition({80.f * scale, 80.f * scale});

    m_hint.setFont(font);
    m_hint.setString("A sound-based survival framework prototype");
    m_hint.setCharacterSize(static_cast<unsigned int>(20.f * scale));
    m_hint.setFillColor(sf::Color(150, 150, 170));
    m_hint.setPosition({84.f * scale, 150.f * scale});

    m_menu.setScale(scale);
    m_menu.addButton(font, "New Game", [this] { m_game.startNewGame(); });
    m_menu.addButton(font, "Continue", [] {
        // TODO: Use SaveSystem::loadWorld/loadPlayerState when save slots exist.
    });
    m_menu.addButton(font, "Settings", [this] { m_game.showSettings(); });
    m_menu.addButton(font, "Quit", [this] { m_game.quitToDesktop(); });
    m_menu.setPosition({88.f * scale, 240.f * scale});
}

void MainMenuState::handleEvent(const sf::Event& event) {
    m_menu.handleEvent(event, m_game.window());
}

void MainMenuState::update(float) {}

void MainMenuState::draw(sf::RenderWindow& window) {
    window.draw(m_title);
    window.draw(m_hint);
    m_menu.draw(window);
}

} // namespace abyss
