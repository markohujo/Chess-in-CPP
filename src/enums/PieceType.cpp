#include "PieceType.h"

std::ostream & operator<<(std::ostream & out, PieceType type) {
    switch (type) {
        case PieceType::PAWN:
            return out << "Pawn";
        case PieceType::ROOK:
            return out << "Rook";
        case PieceType::KNIGHT:
            return out << "Knight";
        case PieceType::BISHOP:
            return out << "Bishop";
        case PieceType::QUEEN:
            return out << "Queen";
        case PieceType::KING:
            return out << "King";
        default:
            return out;
    }
}

std::string to_string(PieceType type) {
    switch (type) {
        case PieceType::PAWN:
            return "Pawn";
        case PieceType::ROOK:
            return "Rook";
        case PieceType::KNIGHT:
            return "Knight";
        case PieceType::BISHOP:
            return "Bishop";
        case PieceType::QUEEN:
            return "Queen";
        case PieceType::KING:
            return "King";
        default:
            return "";
    }
}