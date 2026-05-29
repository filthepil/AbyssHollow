#pragma once

#include "Core/State.h"
#include "UI/MenuList.h"

#include <SFML/Graphics/Text.hpp>

namespace abyss {

class MainMenuState : public State {
public:
    explicit MainMenuState(Game& game);
    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void draw(sf::RenderWindow& window) override;
    void onDisplayChanged() override;

private:
    void rebuildLayout();
    sf::Text m_title;
    sf::Text m_hint;
    ui::MenuList m_menu;
};

} // namespace abyss
