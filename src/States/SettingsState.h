#pragma once

#include "Core/State.h"
#include "UI/Button.h"
#include "UI/Slider.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include <vector>

namespace abyss {

class SettingsState : public State {
public:
    explicit SettingsState(Game& game);
    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void draw(sf::RenderWindow& window) override;

private:
    void rebuildLabels();
    void cycleResolution();
    void cycleDisplayMode();
    void applyAndClose();

    std::vector<sf::Vector2u> m_resolutions;
    std::size_t m_resolutionIndex = 0;
    DisplayMode m_displayMode;

    sf::RectangleShape m_panel;
    sf::Text m_title;
    sf::Text m_resolutionLabel;
    sf::Text m_displayModeLabel;
    std::unique_ptr<ui::Button> m_resolutionButton;
    std::unique_ptr<ui::Button> m_displayModeButton;
    std::unique_ptr<ui::Button> m_applyButton;
    std::unique_ptr<ui::Button> m_backButton;
    ui::Slider m_mainVolume;
    ui::Slider m_playerVolume;
};

} // namespace abyss
