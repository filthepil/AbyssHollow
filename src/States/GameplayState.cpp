#include "States/GameplayState.h"

#include "Core/Game.h"

namespace abyss {

GameplayState::GameplayState(Game& game)
    : State(game) {
    placePlayerAtScreenCenter();
}

void GameplayState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        m_game.showPauseMenu();
    }
    // TODO: Forward movement/echolocation input to PlayerSystem.
}

void GameplayState::update(float deltaSeconds) {
    m_player.update(deltaSeconds);
    m_world.update(deltaSeconds);
}

void GameplayState::draw(sf::RenderWindow& window) {
    m_player.draw(window);
}

void GameplayState::onDisplayChanged() {
    placePlayerAtScreenCenter();
}

void GameplayState::placePlayerAtScreenCenter() {
    const sf::Vector2u size = m_game.window().getSize();
    m_player.setSpawnPosition({static_cast<float>(size.x) * 0.5f, static_cast<float>(size.y) * 0.5f});
}

} // namespace abyss
