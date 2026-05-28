#pragma once

namespace abyss {

// Future home for floor generation, tile collision, exits to upper levels, and
// echolocation raycast geometry. The first iteration intentionally renders black.
class WorldSystem {
public:
    void update(float deltaSeconds);
};

} // namespace abyss
