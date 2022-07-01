#include "GameMode.h"

std::ostream & operator<<(std::ostream & out, GameMode mode) {
    switch (mode) {
        case GameMode::VERSUS:
            return out << 0;
        case GameMode::EASY:
            return out << 1;
        case GameMode::MEDIUM:
            return out << 2;
        case GameMode::HARD:
            return out << 3;
        default:
            return out;
    }
}