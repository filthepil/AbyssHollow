#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace abyss {

class Game;

// Base class for each high-level screen. Future gameplay, editor, loading,
// and cinematic states can be added without changing the main loop.
class State {
public:
    explicit State(Game& game) : m_game(game) {}
    virtual ~State() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float deltaSeconds) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void onDisplayChanged() {}

protected:
    Game& m_game;
};

} // namespace abyss
