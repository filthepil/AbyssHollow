#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

namespace abyss {

class PlayerSystem {
public:
    PlayerSystem();

    void setSpawnPosition(sf::Vector2f position);
    void update(float deltaSeconds);
    void draw(sf::RenderWindow& window) const;

    sf::Vector2f position() const;
    sf::FloatRect bounds() const;

private:
    sf::RectangleShape m_placeholder;
    float m_pulseTimer = 0.f;
    float m_moveSpeed = 220.f;
};

} // namespace abyss
