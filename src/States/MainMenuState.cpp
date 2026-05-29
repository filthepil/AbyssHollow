#include "States/MainMenuState.h"

#include "Core/Game.h"

#include <algorithm>

namespace abyss {

MainMenuState::MainMenuState(Game& game)
    : State(game) {
    rebuildLayout();
}

void MainMenuState::handleEvent(const sf::Event& event) {
    m_menu.handleEvent(event, m_game.window());
}

void MainMenuState::update(float) {}

void MainMenuState::onDisplayChanged() {
    rebuildLayout();
}

void MainMenuState::draw(sf::RenderWindow& window) {
    window.draw(m_title);
    window.draw(m_hint);
    m_menu.draw(window);
}

void MainMenuState::rebuildLayout() {
    const auto& font = m_game.resources().font("default");
    const float scale = m_game.uiScale();
    const sf::Vector2u windowSize = m_game.window().getSize();
    const float width = static_cast<float>(windowSize.x);
    const float height = static_cast<float>(windowSize.y);
    const float margin = 48.f * scale;

    m_title.setFont(font);
    m_title.setString("ABYSS HOLLOW");
    m_title.setCharacterSize(static_cast<unsigned int>(58.f * scale));
    m_title.setFillColor(sf::Color(225, 230, 255));

    m_hint.setFont(font);
    m_hint.setString("A sound-based survival framework prototype");
    m_hint.setCharacterSize(static_cast<unsigned int>(20.f * scale));
    m_hint.setFillColor(sf::Color(150, 150, 170));

    const float contentWidth = 420.f * scale;
    const float contentX = std::max(margin, (width - contentWidth) * 0.5f);
    const float titleY = std::max(margin, height * 0.12f);
    m_title.setPosition({contentX, titleY});
    m_hint.setPosition({contentX + 4.f * scale, titleY + 70.f * scale});

    m_menu = ui::MenuList{};
    m_menu.setScale(scale);
    m_menu.addButton(font, "New Game", [this] { m_game.startNewGame(); });
    m_menu.addButton(font, "Continue", [] {
        // TODO: Use SaveSystem::loadWorld/loadPlayerState when save slots exist.
    });
    m_menu.addButton(font, "Settings", [this] { m_game.showSettings(); });
    m_menu.addButton(font, "Quit", [this] { m_game.quitToDesktop(); });

    const float menuY = std::min(height - 320.f * scale, titleY + 160.f * scale);
    m_menu.setPosition({contentX, std::max(titleY + 120.f * scale, menuY)});
}

} // namespace abyss
