#include "Core/StateStack.h"

namespace abyss {

void StateStack::push(std::unique_ptr<State> state) {
    m_states.push_back(std::move(state));
}

void StateStack::pop() {
    if (!m_states.empty()) {
        m_states.pop_back();
    }
}

void StateStack::clear() {
    m_states.clear();
}

State* StateStack::top() {
    return m_states.empty() ? nullptr : m_states.back().get();
}

const State* StateStack::top() const {
    return m_states.empty() ? nullptr : m_states.back().get();
}

bool StateStack::empty() const {
    return m_states.empty();
}

} // namespace abyss
