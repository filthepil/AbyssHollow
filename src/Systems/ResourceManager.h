#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <string>

namespace abyss {

// Central place for external assets. Nothing visual is baked into code; future
// textures, sprite atlases, UI skins, and creature art should be requested here.
class ResourceManager {
public:
    bool loadFont(const std::string& id, const std::string& path);
    const sf::Font& font(const std::string& id) const;

    bool loadTexture(const std::string& id, const std::string& path);
    const sf::Texture& texture(const std::string& id) const;

private:
    std::map<std::string, sf::Font> m_fonts;
    std::map<std::string, sf::Texture> m_textures;
};

} // namespace abyss
