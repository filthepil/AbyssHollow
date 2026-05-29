#pragma once

#include "Core/State.h"

#include <memory>
#include <vector>

namespace abyss {

// Lightweight stack of states. The game state can be covered by the pause
// menu, while settings can be pushed over either the main or pause menu.
class StateStack {
public:
    void push(std::unique_ptr<State> state);
    void pop();
    void clear();
    State* top();
    const State* top() const;
    void onDisplayChanged();
    bool empty() const;

private:
    std::vector<std::unique_ptr<State>> m_states;
};

} // namespace abyss
