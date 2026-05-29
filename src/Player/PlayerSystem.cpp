#include "Player/PlayerSystem.h"

#include <SFML/Graphics/Color.hpp>

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
    m_pulseTimer += std::max(0.f, deltaSeconds);
    const float wave = (std::cos((m_pulseTimer / PulsePeriodSeconds) * 2.f * Pi) + 1.f) * 0.5f;
    const auto red = static_cast<sf::Uint8>(std::round(255.f * wave));
    m_placeholder.setFillColor(sf::Color(red, 0, 0));
}

void PlayerSystem::draw(sf::RenderWindow& window) const {
    window.draw(m_placeholder);
}

} // namespace abyss
