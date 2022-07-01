#pragma once

#include <ostream>

/** Represents of type of game is played (1v1, single player - easy bot, medium bot, hard bot) */
enum class GameMode {
    VERSUS, EASY, MEDIUM, HARD
};

std::ostream & operator<<(std::ostream & out, GameMode mode);
