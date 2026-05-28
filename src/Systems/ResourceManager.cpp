#include "Systems/ResourceManager.h"

#include <stdexcept>

namespace abyss {

bool ResourceManager::loadFont(const std::string& id, const std::string& path) {
    sf::Font font;
    if (!font.loadFromFile(path)) {
        return false;
    }
    m_fonts[id] = std::move(font);
    return true;
}

const sf::Font& ResourceManager::font(const std::string& id) const {
    auto found = m_fonts.find(id);
    if (found == m_fonts.end()) {
        throw std::runtime_error("Missing font resource: " + id);
    }
    return found->second;
}

bool ResourceManager::loadTexture(const std::string& id, const std::string& path) {
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        return false;
    }
    m_textures[id] = std::move(texture);
    return true;
}

const sf::Texture& ResourceManager::texture(const std::string& id) const {
    auto found = m_textures.find(id);
    if (found == m_textures.end()) {
        throw std::runtime_error("Missing texture resource: " + id);
    }
    return found->second;
}

} // namespace abyss
