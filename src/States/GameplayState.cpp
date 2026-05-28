#include "States/GameplayState.h"

#include "Core/Game.h"

namespace abyss {

GameplayState::GameplayState(Game& game)
    : State(game) {}

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

void GameplayState::draw(sf::RenderWindow&) {
    // Intentionally black for the first iteration. Future rendering should draw
    // only external textures/sprites and the player's visible heart.
}

} // namespace abyss
