#pragma once

#include "Core/Settings.h"
#include "Core/StateStack.h"
#include "Systems/AudioSystem.h"
#include "Systems/ResourceManager.h"
#include "Systems/SaveSystem.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace abyss {

class Game {
public:
    Game();
    void run();

    sf::RenderWindow& window();
    ResourceManager& resources();
    AudioSystem& audio();
    SaveSystem& saves();
    Settings& settings();
    float uiScale() const;

    void showMainMenu();
    void startNewGame();
    void showPauseMenu();
    void showSettings();
    void closeCurrentState();
    void quitToDesktop();
    void applyVideoSettings();

private:
    void initializeDisplaySettings();
    void createWindow();
    void loadCoreAssets();

    Settings m_settings;
    sf::RenderWindow m_window;
    StateStack m_states;
    ResourceManager m_resources;
    AudioSystem m_audio;
    SaveSystem m_saves;
    bool m_running = true;
};

} // namespace abyss
