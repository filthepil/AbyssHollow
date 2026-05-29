#pragma once

#include "UI/Button.h"

#include <memory>
#include <vector>

namespace abyss::ui {

class MenuList {
public:
    void setScale(float uiScale);
    Button& addButton(const sf::Font& font, const std::string& label, std::function<void()> callback);
    void setPosition(sf::Vector2f origin);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;

private:
    std::vector<std::unique_ptr<Button>> m_buttons;
    sf::Vector2f m_origin{0.f, 0.f};
    sf::Vector2f m_buttonSize{360.f, 54.f};
    float m_spacing = 16.f;
    float m_uiScale = 1.f;
};

} // namespace abyss::ui
