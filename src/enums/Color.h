#pragma once

#include <ostream>

/** Represents color of chess pieces */
enum class Color {
    WHITE, BLACK
};

std::ostream & operator<<(std::ostream & out, Color color);

/** @return the other color */
Color operator!(Color color);

std::string to_string(Color color);
