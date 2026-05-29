#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace abyss {

// Future home for movement, unseen body rules, visible heart rendering, and the
// player-owned echolocation pulse emitter.
class PlayerSystem {
public:
    PlayerSystem();

    void setSpawnPosition(sf::Vector2f position);
    void update(float deltaSeconds);
    void draw(sf::RenderWindow& window) const;

private:
    sf::RectangleShape m_placeholder;
    float m_pulseTimer = 0.f;
};

} // namespace abyss
