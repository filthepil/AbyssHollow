#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>
#include <vector>

namespace abyss {

struct LevelChunk {
    std::string id;
    sf::Vector2i originTiles{0, 0};
    sf::Vector2i sizeTiles{0, 0};
    std::vector<std::string> tiles;
};

struct TeleportExit {
    sf::Vector2i positionTiles{0, 0};
    sf::Vector2i sizeTiles{1, 1};
    std::string targetLevel;
    sf::Vector2i targetSpawnTiles{0, 0};
};

struct LevelData {
    std::string id;
    std::string name;
    int widthTiles = 0;
    int heightTiles = 0;
    int tileSize = 32;
    std::string spawnId = "start";
    sf::Vector2i playerSpawnTiles{0, 0};
    std::vector<LevelChunk> chunks;
    std::vector<TeleportExit> teleportExits;
};

bool loadLevelFile(const std::string& path, LevelData& level, std::string& errorMessage);
sf::Vector2f tileCenterToWorld(sf::Vector2i tilePosition, int tileSize);
sf::FloatRect tileRectToWorld(sf::Vector2i tilePosition, sf::Vector2i tileSize, int pixelsPerTile);

} // namespace abyss
