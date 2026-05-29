#include "States/GameplayState.h"

#include "Core/Game.h"

#include <SFML/Window/Keyboard.hpp>

namespace abyss {
namespace {

constexpr const char* StarterLevel = "level_01.json";

} // namespace

GameplayState::GameplayState(Game& game)
    : State(game) {
    onDisplayChanged();
    loadStarterLevel();
    updateCameraView();
}

void GameplayState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        m_game.showPauseMenu();
    }
}

void GameplayState::update(float deltaSeconds) {
    m_player.update(deltaSeconds);
    m_world.update(deltaSeconds);

    if (const TeleportExit* exit = m_world.findTouchedTeleport(m_player.bounds())) {
        teleportTo(*exit);
    }

    updateCameraView();
}

void GameplayState::draw(sf::RenderWindow& window) {
    const sf::View previousView = window.getView();
    window.setView(m_cameraView);
    m_world.draw(window);
    m_player.draw(window);
    window.setView(previousView);
}

void GameplayState::onDisplayChanged() {
    const sf::Vector2u size = m_game.window().getSize();
    m_cameraView.setSize({static_cast<float>(size.x), static_cast<float>(size.y)});
    updateCameraView();
}

void GameplayState::loadStarterLevel() {
    if (m_world.loadLevel(StarterLevel)) {
        m_player.setSpawnPosition(m_world.currentSpawnWorldPosition());
    }
}

void GameplayState::teleportTo(const TeleportExit& exit) {
    const std::string targetLevel = exit.targetLevel;
    const sf::Vector2i targetSpawnTiles = exit.targetSpawnTiles;
    if (m_world.loadLevel(targetLevel)) {
        m_player.setSpawnPosition(tileCenterToWorld(targetSpawnTiles, m_world.level().tileSize));
    }
}

void GameplayState::updateCameraView() {
    m_cameraView.setCenter(m_player.position());
}

} // namespace abyss
