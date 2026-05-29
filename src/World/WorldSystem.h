#pragma once

#include "World/LevelData.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>

namespace abyss {

class WorldSystem {
public:
    bool loadLevel(const std::string& levelFile);
    void update(float deltaSeconds);
    void draw(sf::RenderWindow& window) const;

    const LevelData& level() const;
    const std::string& currentLevelFile() const;
    sf::Vector2f currentSpawnWorldPosition() const;
    const TeleportExit* findTouchedTeleport(sf::FloatRect playerBounds) const;

private:
    bool isChunkVisible(const LevelChunk& chunk, const sf::FloatRect& cameraBounds) const;
    void drawChunk(sf::RenderWindow& window, const LevelChunk& chunk) const;
    sf::FloatRect paddedCameraBounds(const sf::RenderWindow& window) const;

    LevelData m_level;
    std::string m_currentLevelFile;
    mutable sf::RectangleShape m_tileShape;
};

} // namespace abyss
