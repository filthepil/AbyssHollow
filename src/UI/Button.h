#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Mouse.hpp>
#include <functional>
#include <string>

namespace abyss::ui {

class Button {
public:
    Button(const sf::Font& font, std::string label, sf::Vector2f size, float uiScale = 1.f);

    void setPosition(sf::Vector2f position);
    void setCallback(std::function<void()> callback);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;

private:
    void updateTextPosition();

    float m_uiScale = 1.f;
    sf::RectangleShape m_shape;
    sf::Text m_text;
    std::function<void()> m_callback;
};

} // namespace abyss::ui
