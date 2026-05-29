#include "UI/MenuList.h"

namespace abyss::ui {

void MenuList::setScale(float uiScale) {
    m_uiScale = uiScale;
    m_buttonSize = {360.f * m_uiScale, 54.f * m_uiScale};
    m_spacing = 16.f * m_uiScale;
    setPosition(m_origin);
}

Button& MenuList::addButton(const sf::Font& font, const std::string& label, std::function<void()> callback) {
    auto button = std::make_unique<Button>(font, label, m_buttonSize, m_uiScale);
    button->setCallback(std::move(callback));
    Button& reference = *button;
    m_buttons.push_back(std::move(button));
    setPosition(m_origin);
    return reference;
}

void MenuList::setPosition(sf::Vector2f origin) {
    m_origin = origin;
    for (std::size_t i = 0; i < m_buttons.size(); ++i) {
        m_buttons[i]->setPosition({m_origin.x, m_origin.y + static_cast<float>(i) * (m_buttonSize.y + m_spacing)});
    }
}

void MenuList::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    for (auto& button : m_buttons) {
        button->handleEvent(event, window);
    }
}

void MenuList::draw(sf::RenderWindow& window) const {
    for (const auto& button : m_buttons) {
        button->draw(window);
    }
}

} // namespace abyss::ui
