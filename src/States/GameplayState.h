#pragma once

#include "Core/State.h"
#include "Player/PlayerSystem.h"
#include "World/WorldSystem.h"

namespace abyss {

class GameplayState : public State {
public:
    explicit GameplayState(Game& game);
    void handleEvent(const sf::Event& event) override;
    void update(float deltaSeconds) override;
    void draw(sf::RenderWindow& window) override;
    void onDisplayChanged() override;

private:
    void placePlayerAtScreenCenter();
    PlayerSystem m_player;
    WorldSystem m_world;
};

} // namespace abyss
