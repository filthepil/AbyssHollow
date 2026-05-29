#include "Player/PlayerSystem.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <cmath>

namespace abyss {
namespace {

constexpr float Pi = 3.14159265358979323846f;
constexpr float PulsePeriodSeconds = 1.4f;

} // namespace

PlayerSystem::PlayerSystem() {
    m_placeholder.setSize({48.f, 48.f});
    m_placeholder.setOrigin({24.f, 24.f});
    m_placeholder.setFillColor(sf::Color::Red);
}

void PlayerSystem::setSpawnPosition(sf::Vector2f position) {
    m_placeholder.setPosition(position);
}

void PlayerSystem::update(float deltaSeconds) {
    const float safeDelta = std::max(0.f, deltaSeconds);

    sf::Vector2f movement{0.f, 0.f};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        movement.x -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        movement.x += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        movement.y -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        movement.y += 1.f;
    }

    const float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    if (length > 0.f) {
        movement.x /= length;
        movement.y /= length;
        m_placeholder.move(movement * m_moveSpeed * safeDelta);
    }

    m_pulseTimer += safeDelta;
    const float wave = (std::cos((m_pulseTimer / PulsePeriodSeconds) * 2.f * Pi) + 1.f) * 0.5f;
    const auto red = static_cast<sf::Uint8>(std::round(255.f * wave));
    m_placeholder.setFillColor(sf::Color(red, 0, 0));
}

void PlayerSystem::draw(sf::RenderWindow& window) const {
    window.draw(m_placeholder);
}

sf::Vector2f PlayerSystem::position() const { return m_placeholder.getPosition(); }
sf::FloatRect PlayerSystem::bounds() const { return m_placeholder.getGlobalBounds(); }

} // namespace abyss
