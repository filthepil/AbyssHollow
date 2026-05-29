#include "World/WorldSystem.h"

#include <SFML/Graphics/View.hpp>

#include <iostream>
#include <utility>

namespace abyss {
namespace {

constexpr const char* LevelDirectory = "levels/";
constexpr float ChunkVisibilityPaddingTiles = 2.f;
const sf::Color BackgroundTileColor(72, 72, 72);
const sf::Color AlternateTileColor(78, 78, 78);
const sf::Color GridColor(105, 105, 105);
const sf::Color TeleportColor(80, 120, 220, 120);

} // namespace

bool WorldSystem::loadLevel(const std::string& levelFile) {
    LevelData loadedLevel;
    std::string errorMessage;
    if (!loadLevelFile(std::string(LevelDirectory) + levelFile, loadedLevel, errorMessage)) {
        std::cerr << "Failed to load level '" << levelFile << "': " << errorMessage << '\n';
        return false;
    }

    m_level = std::move(loadedLevel);
    m_currentLevelFile = levelFile;
    m_tileShape.setSize({static_cast<float>(m_level.tileSize), static_cast<float>(m_level.tileSize)});
    m_tileShape.setOutlineThickness(1.f);
    m_tileShape.setOutlineColor(GridColor);
    return true;
}

void WorldSystem::update(float) {
    // TODO: Advance floor hazards, tile animation, and future level scripts.
}

void WorldSystem::draw(sf::RenderWindow& window) const {
    if (m_level.tileSize <= 0) {
        return;
    }

    const sf::FloatRect cameraBounds = paddedCameraBounds(window);
    for (const LevelChunk& chunk : m_level.chunks) {
        if (isChunkVisible(chunk, cameraBounds)) {
            drawChunk(window, chunk);
        }
    }

    sf::RectangleShape teleportShape;
    teleportShape.setFillColor(TeleportColor);
    teleportShape.setOutlineColor(sf::Color(120, 170, 255));
    teleportShape.setOutlineThickness(2.f);
    for (const TeleportExit& exit : m_level.teleportExits) {
        const sf::FloatRect bounds = tileRectToWorld(exit.positionTiles, exit.sizeTiles, m_level.tileSize);
        if (cameraBounds.intersects(bounds)) {
            teleportShape.setPosition({bounds.left, bounds.top});
            teleportShape.setSize({bounds.width, bounds.height});
            window.draw(teleportShape);
        }
    }
}

const LevelData& WorldSystem::level() const { return m_level; }
const std::string& WorldSystem::currentLevelFile() const { return m_currentLevelFile; }

sf::Vector2f WorldSystem::currentSpawnWorldPosition() const {
    return tileCenterToWorld(m_level.playerSpawnTiles, m_level.tileSize);
}

const TeleportExit* WorldSystem::findTouchedTeleport(sf::FloatRect playerBounds) const {
    for (const TeleportExit& exit : m_level.teleportExits) {
        if (playerBounds.intersects(tileRectToWorld(exit.positionTiles, exit.sizeTiles, m_level.tileSize))) {
            return &exit;
        }
    }
    return nullptr;
}

bool WorldSystem::isChunkVisible(const LevelChunk& chunk, const sf::FloatRect& cameraBounds) const {
    const sf::FloatRect chunkBounds = tileRectToWorld(chunk.originTiles, chunk.sizeTiles, m_level.tileSize);
    return cameraBounds.intersects(chunkBounds);
}

void WorldSystem::drawChunk(sf::RenderWindow& window, const LevelChunk& chunk) const {
    const int rowCount = static_cast<int>(chunk.tiles.size());
    for (int row = 0; row < rowCount; ++row) {
        const int columnCount = static_cast<int>(chunk.tiles[row].size());
        for (int column = 0; column < columnCount; ++column) {
            m_tileShape.setPosition({static_cast<float>((chunk.originTiles.x + column) * m_level.tileSize),
                                     static_cast<float>((chunk.originTiles.y + row) * m_level.tileSize)});
            const bool alternate = ((chunk.originTiles.x + column) + (chunk.originTiles.y + row)) % 2 == 0;
            m_tileShape.setFillColor(alternate ? BackgroundTileColor : AlternateTileColor);
            window.draw(m_tileShape);
        }
    }
}

sf::FloatRect WorldSystem::paddedCameraBounds(const sf::RenderWindow& window) const {
    const sf::View& view = window.getView();
    const sf::Vector2f viewCenter = view.getCenter();
    const sf::Vector2f viewSize = view.getSize();
    const float padding = static_cast<float>(m_level.tileSize) * ChunkVisibilityPaddingTiles;
    return {viewCenter.x - viewSize.x * 0.5f - padding, viewCenter.y - viewSize.y * 0.5f - padding,
            viewSize.x + padding * 2.f, viewSize.y + padding * 2.f};
}

} // namespace abyss
