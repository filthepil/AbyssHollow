#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <functional>
#include <string>

namespace abyss::ui {

// Slider with direct numeric typing. Click the number box, type digits, and
// press Enter. This is cosmetic for now and intentionally not wired to audio.
class Slider {
public:
    Slider(const sf::Font& font, std::string label, float min, float max, float value);

    void setPosition(sf::Vector2f position);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;
    void setValue(float value);
    float value() const;

private:
    void applyMouseValue(float mouseX);
    void refreshText();
    void commitTypedValue();

    float m_min = 0.f;
    float m_max = 100.f;
    float m_value = 0.f;
    bool m_dragging = false;
    bool m_typing = false;
    std::string m_typedValue;

    sf::Text m_label;
    sf::RectangleShape m_track;
    sf::RectangleShape m_fill;
    sf::CircleShape m_knob;
    sf::RectangleShape m_inputBox;
    sf::Text m_valueText;
};

} // namespace abyss::ui
