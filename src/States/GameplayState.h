#pragma once

#include "Core/State.h"
#include "Player/PlayerSystem.h"
#include "World/WorldSystem.h"

#include <SFML/Graphics/View.hpp>

namespace abyss {

class GameplayState : public State {
public:
    explicit GameplayState(Game& game);
    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void draw(sf::RenderWindow& window) override;
    void onDisplayChanged() override;

private:
    void loadStarterLevel();
    void teleportTo(const TeleportExit& exit);
    void updateCameraView();

    PlayerSystem m_player;
    WorldSystem m_world;
    sf::View m_cameraView;
};

} // namespace abyss
