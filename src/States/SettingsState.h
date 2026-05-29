#pragma once

#include "Core/State.h"
#include "Core/Settings.h"
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
    enum class FocusedInput {
        None,
        Width,
        Height
    };

    enum class MenuPage {
        Settings,
        Controls
    };

    void addResolution(sf::Vector2u resolution);
    void rebuildLabels();
    void cycleDisplayMode();
    void applyAndClose();
    void openControlsMenu();
    void closeControlsMenu();
    bool validateCustomResolution(sf::Vector2u& resolution);
    void refreshResolutionInput();
    void setInputFocus(FocusedInput input);
    void handleTextInput(const sf::Event& event);
    void positionTextInBox(sf::Text& text, const sf::RectangleShape& box);
    std::string resolutionText(sf::Vector2u resolution) const;
    float scaled(float value) const;

    std::vector<sf::Vector2u> m_resolutions;
    std::size_t m_resolutionIndex = 0;
    DisplayMode m_displayMode;
    float m_scale = 1.f;
    bool m_resolutionDropdownOpen = false;
    bool m_userChangedUiScale = false;
    bool m_closeAfterEvent = false;
    MenuPage m_page = MenuPage::Settings;
    FocusedInput m_focusedInput = FocusedInput::None;
    std::string m_widthInput;
    std::string m_heightInput;

    sf::RectangleShape m_panel;
    sf::Text m_title;
    sf::Text m_resolutionLabel;
    sf::Text m_resolutionValue;
    sf::RectangleShape m_resolutionBox;
    sf::Text m_customLabel;
    sf::Text m_widthText;
    sf::Text m_heightText;
    sf::Text m_byText;
    sf::RectangleShape m_widthBox;
    sf::RectangleShape m_heightBox;
    sf::Text m_displayModeLabel;
    sf::Text m_errorText;
    std::unique_ptr<ui::Button> m_displayModeButton;
    std::unique_ptr<ui::Button> m_controlsButton;
    std::unique_ptr<ui::Button> m_applyButton;
    std::unique_ptr<ui::Button> m_backButton;
    std::unique_ptr<ui::Button> m_controlsApplyButton;
    std::unique_ptr<ui::Button> m_controlsCancelButton;
    ui::Slider m_uiScaleSlider;
    ui::Slider m_mainVolume;
    ui::Slider m_playerVolume;
};

} // namespace abyss
