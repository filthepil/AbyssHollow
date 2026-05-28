#include "States/PauseMenuState.h"

#include "Core/Game.h"

namespace abyss {

PauseMenuState::PauseMenuState(Game& game)
    : State(game) {
    const auto& font = m_game.resources().font("default");

    m_panel.setSize({500.f, 440.f});
    m_panel.setPosition({70.f, 70.f});
    m_panel.setFillColor(sf::Color(8, 8, 12, 235));
    m_panel.setOutlineColor(sf::Color(90, 90, 115));
    m_panel.setOutlineThickness(2.f);

    m_title.setFont(font);
    m_title.setString("Paused");
    m_title.setCharacterSize(42);
    m_title.setFillColor(sf::Color::White);
    m_title.setPosition({100.f, 100.f});

    m_menu.addButton(font, "Return to game", [this] { m_game.closeCurrentState(); });
    m_menu.addButton(font, "Quit to main menu", [this] { m_game.showMainMenu(); });
    m_menu.addButton(font, "Quit to desktop", [this] { m_game.quitToDesktop(); });
    m_menu.addButton(font, "Settings", [this] { m_game.showSettings(); });
    m_menu.setPosition({110.f, 180.f});
}

void PauseMenuState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        m_game.closeCurrentState();
        return;
    }
    m_menu.handleEvent(event, m_game.window());
}

void PauseMenuState::update(float) {}

void PauseMenuState::draw(sf::RenderWindow& window) {
    window.draw(m_panel);
    window.draw(m_title);
    m_menu.draw(window);
}

} // namespace abyss
