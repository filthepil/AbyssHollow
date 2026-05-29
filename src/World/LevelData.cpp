#include "World/LevelData.h"

#include <SFML/Graphics/Rect.hpp>

#include <cctype>
#include <exception>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace abyss {
namespace {

struct JsonValue {
    enum class Type { Null, Number, String, Object, Array };

    Type type = Type::Null;
    double number = 0.0;
    std::string string;
    std::map<std::string, JsonValue> object;
    std::vector<JsonValue> array;
};

class JsonParser {
public:
    explicit JsonParser(std::string text)
        : m_text(std::move(text)) {}

    JsonValue parse() {
        JsonValue value = parseValue();
        skipWhitespace();
        if (!isAtEnd()) {
            fail("unexpected trailing content");
        }
        return value;
    }

private:
    JsonValue parseValue() {
        skipWhitespace();
        if (isAtEnd()) {
            fail("unexpected end of file");
        }

        const char c = peek();
        if (c == '{') {
            return parseObject();
        }
        if (c == '[') {
            return parseArray();
        }
        if (c == '"') {
            JsonValue value;
            value.type = JsonValue::Type::String;
            value.string = parseString();
            return value;
        }
        if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
            return parseNumber();
        }
        if (matchLiteral("null")) {
            return JsonValue{};
        }

        fail("expected JSON value");
    }

    JsonValue parseObject() {
        JsonValue value;
        value.type = JsonValue::Type::Object;
        consume('{');
        skipWhitespace();
        if (consumeIf('}')) {
            return value;
        }

        while (true) {
            skipWhitespace();
            if (peek() != '"') {
                fail("expected object key");
            }
            std::string key = parseString();
            skipWhitespace();
            consume(':');
            value.object.emplace(std::move(key), parseValue());
            skipWhitespace();
            if (consumeIf('}')) {
                return value;
            }
            consume(',');
        }
    }

    JsonValue parseArray() {
        JsonValue value;
        value.type = JsonValue::Type::Array;
        consume('[');
        skipWhitespace();
        if (consumeIf(']')) {
            return value;
        }

        while (true) {
            value.array.push_back(parseValue());
            skipWhitespace();
            if (consumeIf(']')) {
                return value;
            }
            consume(',');
        }
    }

    JsonValue parseNumber() {
        const std::size_t start = m_position;
        consumeIf('-');
        while (!isAtEnd() && std::isdigit(static_cast<unsigned char>(peek()))) {
            ++m_position;
        }
        if (!isAtEnd() && peek() == '.') {
            ++m_position;
            while (!isAtEnd() && std::isdigit(static_cast<unsigned char>(peek()))) {
                ++m_position;
            }
        }

        JsonValue value;
        value.type = JsonValue::Type::Number;
        value.number = std::stod(m_text.substr(start, m_position - start));
        return value;
    }

    std::string parseString() {
        consume('"');
        std::string result;
        while (!isAtEnd()) {
            const char c = advance();
            if (c == '"') {
                return result;
            }
            if (c == '\\') {
                if (isAtEnd()) {
                    fail("unterminated escape sequence");
                }
                const char escaped = advance();
                switch (escaped) {
                case '"':
                case '\\':
                case '/':
                    result.push_back(escaped);
                    break;
                case 'n':
                    result.push_back('\n');
                    break;
                case 'r':
                    result.push_back('\r');
                    break;
                case 't':
                    result.push_back('\t');
                    break;
                default:
                    fail("unsupported string escape");
                }
            } else {
                result.push_back(c);
            }
        }
        fail("unterminated string");
    }

    bool matchLiteral(const std::string& literal) {
        if (m_text.compare(m_position, literal.size(), literal) != 0) {
            return false;
        }
        m_position += literal.size();
        return true;
    }

    void skipWhitespace() {
        while (!isAtEnd() && std::isspace(static_cast<unsigned char>(peek()))) {
            ++m_position;
        }
    }

    bool consumeIf(char expected) {
        if (isAtEnd() || peek() != expected) {
            return false;
        }
        ++m_position;
        return true;
    }

    void consume(char expected) {
        if (!consumeIf(expected)) {
            std::ostringstream message;
            message << "expected '" << expected << "'";
            fail(message.str());
        }
    }

    char advance() { return m_text[m_position++]; }
    char peek() const { return m_text[m_position]; }
    bool isAtEnd() const { return m_position >= m_text.size(); }

    [[noreturn]] void fail(const std::string& message) const {
        std::ostringstream error;
        error << "JSON parse error at byte " << m_position << ": " << message;
        throw std::runtime_error(error.str());
    }

    std::string m_text;
    std::size_t m_position = 0;
};

const JsonValue& objectField(const JsonValue& value, const std::string& name) {
    if (value.type != JsonValue::Type::Object) {
        throw std::runtime_error("expected an object while reading field '" + name + "'");
    }
    const auto found = value.object.find(name);
    if (found == value.object.end()) {
        throw std::runtime_error("missing required field '" + name + "'");
    }
    return found->second;
}

std::string stringField(const JsonValue& value, const std::string& name) {
    const JsonValue& field = objectField(value, name);
    if (field.type != JsonValue::Type::String) {
        throw std::runtime_error("field '" + name + "' must be a string");
    }
    return field.string;
}

int intField(const JsonValue& value, const std::string& name) {
    const JsonValue& field = objectField(value, name);
    if (field.type != JsonValue::Type::Number) {
        throw std::runtime_error("field '" + name + "' must be a number");
    }
    return static_cast<int>(field.number);
}

sf::Vector2i vector2Field(const JsonValue& value, const std::string& name) {
    const JsonValue& field = objectField(value, name);
    if (field.type != JsonValue::Type::Array || field.array.size() != 2) {
        throw std::runtime_error("field '" + name + "' must be a two-number array");
    }
    if (field.array[0].type != JsonValue::Type::Number || field.array[1].type != JsonValue::Type::Number) {
        throw std::runtime_error("field '" + name + "' must only contain numbers");
    }
    return {static_cast<int>(field.array[0].number), static_cast<int>(field.array[1].number)};
}

bool hasField(const JsonValue& value, const std::string& name) {
    return value.type == JsonValue::Type::Object && value.object.find(name) != value.object.end();
}

LevelChunk parseChunk(const JsonValue& value) {
    LevelChunk chunk;
    chunk.id = stringField(value, "id");
    chunk.originTiles = vector2Field(value, "origin");
    chunk.sizeTiles = vector2Field(value, "size");

    const JsonValue& tiles = objectField(value, "tiles");
    if (tiles.type != JsonValue::Type::Array) {
        throw std::runtime_error("chunk tiles must be an array of strings");
    }
    for (const JsonValue& row : tiles.array) {
        if (row.type != JsonValue::Type::String) {
            throw std::runtime_error("chunk tile rows must be strings");
        }
        chunk.tiles.push_back(row.string);
    }

    return chunk;
}

TeleportExit parseTeleportExit(const JsonValue& value) {
    TeleportExit exit;
    exit.positionTiles = vector2Field(value, "position");
    exit.sizeTiles = vector2Field(value, "size");
    exit.targetLevel = stringField(value, "target_level");
    exit.targetSpawnTiles = vector2Field(value, "target_spawn_position");
    return exit;
}

LevelData parseLevel(const JsonValue& root) {
    if (root.type != JsonValue::Type::Object) {
        throw std::runtime_error("level root must be an object");
    }

    LevelData level;
    level.id = stringField(root, "id");
    level.name = stringField(root, "name");
    level.widthTiles = intField(root, "width");
    level.heightTiles = intField(root, "height");
    level.tileSize = intField(root, "tile_size");

    const JsonValue& spawn = objectField(root, "player_spawn");
    if (hasField(spawn, "id")) {
        level.spawnId = stringField(spawn, "id");
    }
    level.playerSpawnTiles = vector2Field(spawn, "position");

    const JsonValue& chunks = objectField(root, "chunks");
    if (chunks.type != JsonValue::Type::Array) {
        throw std::runtime_error("chunks must be an array");
    }
    for (const JsonValue& chunk : chunks.array) {
        level.chunks.push_back(parseChunk(chunk));
    }

    const JsonValue& exits = objectField(root, "teleport_exits");
    if (exits.type != JsonValue::Type::Array) {
        throw std::runtime_error("teleport_exits must be an array");
    }
    for (const JsonValue& exit : exits.array) {
        level.teleportExits.push_back(parseTeleportExit(exit));
    }

    return level;
}

} // namespace

bool loadLevelFile(const std::string& path, LevelData& level, std::string& errorMessage) {
    std::ifstream file(path);
    if (!file) {
        errorMessage = "could not open level file: " + path;
        return false;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    try {
        level = parseLevel(JsonParser(buffer.str()).parse());
        return true;
    } catch (const std::exception& exception) {
        errorMessage = exception.what();
        return false;
    }
}

sf::Vector2f tileCenterToWorld(sf::Vector2i tilePosition, int tileSize) {
    return {static_cast<float>(tilePosition.x * tileSize) + static_cast<float>(tileSize) * 0.5f,
            static_cast<float>(tilePosition.y * tileSize) + static_cast<float>(tileSize) * 0.5f};
}

sf::FloatRect tileRectToWorld(sf::Vector2i tilePosition, sf::Vector2i tileSize, int pixelsPerTile) {
    return {static_cast<float>(tilePosition.x * pixelsPerTile), static_cast<float>(tilePosition.y * pixelsPerTile),
            static_cast<float>(tileSize.x * pixelsPerTile), static_cast<float>(tileSize.y * pixelsPerTile)};
}

} // namespace abyss
