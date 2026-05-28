#include "Core/Game.h"

#include <exception>
#include <iostream>

int main() {
    try {
        abyss::Game game;
        game.run();
    } catch (const std::exception& error) {
        std::cerr << "Abyss Hollow crashed: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
