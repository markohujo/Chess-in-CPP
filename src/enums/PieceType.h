#pragma once

#include <ostream>

enum class PieceType {
    PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING
};

std::ostream & operator<<(std::ostream & out, PieceType type);

std::string to_string(PieceType type);