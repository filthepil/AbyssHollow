#include "UI/Button.h"

namespace abyss::ui {

Button::Button(const sf::Font& font, std::string label, sf::Vector2f size) {
    m_shape.setSize(size);
    m_shape.setFillColor(sf::Color(20, 20, 28));
    m_shape.setOutlineColor(sf::Color(120, 120, 145));
    m_shape.setOutlineThickness(2.f);

    m_text.setFont(font);
    m_text.setString(std::move(label));
    m_text.setCharacterSize(26);
    m_text.setFillColor(sf::Color::White);
}

void Button::setPosition(sf::Vector2f position) {
    m_shape.setPosition(position);
    updateTextPosition();
}

void Button::setCallback(std::function<void()> callback) {
    m_callback = std::move(callback);
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    const auto mouse = sf::Vector2f(sf::Mouse::getPosition(window));
    const bool hovered = m_shape.getGlobalBounds().contains(mouse);
    m_shape.setFillColor(hovered ? sf::Color(35, 35, 48) : sf::Color(20, 20, 28));

    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left &&
        hovered && m_callback) {
        m_callback();
    }
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
    window.draw(m_text);
}

void Button::updateTextPosition() {
    const sf::FloatRect bounds = m_text.getLocalBounds();
    const sf::Vector2f buttonPos = m_shape.getPosition();
    const sf::Vector2f buttonSize = m_shape.getSize();
    m_text.setPosition(
        buttonPos.x + (buttonSize.x - bounds.width) / 2.f - bounds.left,
        buttonPos.y + (buttonSize.y - bounds.height) / 2.f - bounds.top - 2.f);
}

} // namespace abyss::ui
