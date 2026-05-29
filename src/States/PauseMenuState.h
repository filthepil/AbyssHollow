#pragma once

#include "Core/State.h"
#include "UI/MenuList.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace abyss {

class PauseMenuState : public State {
public:
    explicit PauseMenuState(Game& game);
    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void draw(sf::RenderWindow& window) override;
    void onDisplayChanged() override;

private:
    void rebuildLayout();
    sf::RectangleShape m_panel;
    sf::Text m_title;
    ui::MenuList m_menu;
};

} // namespace abyss
