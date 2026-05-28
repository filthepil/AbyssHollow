#pragma once

namespace abyss {

// Future home for movement, unseen body rules, visible heart rendering, and the
// player-owned echolocation pulse emitter.
class PlayerSystem {
public:
    void update(float deltaSeconds);
};

} // namespace abyss
