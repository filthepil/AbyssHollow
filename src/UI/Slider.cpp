#include "UI/Slider.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include <algorithm>
#include <cmath>
#include <sstream>

namespace abyss::ui {
namespace {

std::string formatValue(float value) {
    std::ostringstream stream;
    stream << static_cast<int>(std::round(value));
    return stream.str();
}

} // namespace

Slider::Slider(const sf::Font& font, std::string label, float min, float max, float value)
    : m_min(min), m_max(max) {
    m_label.setFont(font);
    m_label.setString(std::move(label));
    m_label.setCharacterSize(22);
    m_label.setFillColor(sf::Color::White);

    m_track.setSize({310.f, 8.f});
    m_track.setFillColor(sf::Color(55, 55, 70));

    m_fill.setSize({0.f, 8.f});
    m_fill.setFillColor(sf::Color(125, 160, 255));

    m_knob.setRadius(10.f);
    m_knob.setOrigin({10.f, 10.f});
    m_knob.setFillColor(sf::Color(210, 220, 255));

    m_inputBox.setSize({74.f, 36.f});
    m_inputBox.setFillColor(sf::Color(18, 18, 24));
    m_inputBox.setOutlineThickness(2.f);
    m_inputBox.setOutlineColor(sf::Color(110, 110, 135));

    m_valueText.setFont(font);
    m_valueText.setCharacterSize(20);
    m_valueText.setFillColor(sf::Color::White);
    setValue(value);
}

void Slider::setPosition(sf::Vector2f position) {
    m_label.setPosition(position);
    m_track.setPosition({position.x, position.y + 42.f});
    m_fill.setPosition(m_track.getPosition());
    m_inputBox.setPosition({position.x + 345.f, position.y + 27.f});
    refreshText();
}

void Slider::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    const auto mouse = sf::Vector2f(sf::Mouse::getPosition(window));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        m_typing = m_inputBox.getGlobalBounds().contains(mouse);
        m_inputBox.setOutlineColor(m_typing ? sf::Color(180, 200, 255) : sf::Color(110, 110, 135));
        if (m_track.getGlobalBounds().contains(mouse) || m_knob.getGlobalBounds().contains(mouse)) {
            m_dragging = true;
            applyMouseValue(mouse.x);
        }
    }

    if (event.type == sf::Event::MouseMoved && m_dragging) {
        applyMouseValue(mouse.x);
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        m_dragging = false;
    }

    if (m_typing && event.type == sf::Event::TextEntered) {
        if (event.text.unicode >= '0' && event.text.unicode <= '9') {
            if (m_typedValue.size() < 3) {
                m_typedValue.push_back(static_cast<char>(event.text.unicode));
            }
        } else if (event.text.unicode == 8 && !m_typedValue.empty()) {
            m_typedValue.pop_back();
        }
        refreshText();
    }

    if (m_typing && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        commitTypedValue();
    }
}

void Slider::draw(sf::RenderWindow& window) const {
    window.draw(m_label);
    window.draw(m_track);
    window.draw(m_fill);
    window.draw(m_knob);
    window.draw(m_inputBox);
    window.draw(m_valueText);
}

void Slider::setValue(float value) {
    m_value = std::clamp(value, m_min, m_max);
    m_typedValue = formatValue(m_value);
    refreshText();
}

float Slider::value() const {
    return m_value;
}

void Slider::applyMouseValue(float mouseX) {
    const float left = m_track.getPosition().x;
    const float t = std::clamp((mouseX - left) / m_track.getSize().x, 0.f, 1.f);
    setValue(m_min + (m_max - m_min) * t);
}

void Slider::refreshText() {
    const float t = (m_value - m_min) / (m_max - m_min);
    m_fill.setSize({m_track.getSize().x * t, m_track.getSize().y});
    m_knob.setPosition({m_track.getPosition().x + m_track.getSize().x * t, m_track.getPosition().y + 4.f});
    m_valueText.setString(m_typing ? m_typedValue : formatValue(m_value));
    const sf::FloatRect bounds = m_valueText.getLocalBounds();
    m_valueText.setPosition(
        m_inputBox.getPosition().x + (m_inputBox.getSize().x - bounds.width) / 2.f - bounds.left,
        m_inputBox.getPosition().y + (m_inputBox.getSize().y - bounds.height) / 2.f - bounds.top - 2.f);
}

void Slider::commitTypedValue() {
    if (!m_typedValue.empty()) {
        setValue(std::stof(m_typedValue));
    }
    m_typing = false;
    m_inputBox.setOutlineColor(sf::Color(110, 110, 135));
    refreshText();
}

} // namespace abyss::ui
