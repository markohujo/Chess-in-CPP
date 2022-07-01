#include "Rook.h"

const char * WHITE_ROOK_PRETTY = "\u2656";
const char WHITE_ROOK = 'R';
const char * BLACK_ROOK_PRETTY = "\u265C";
const char BLACK_ROOK = 'r';

PieceType Rook::type() const {
    return PieceType::ROOK;
}

Rook::Rook(int x, int y, Color color) : Piece(x, y, color) {}

std::list<Move> Rook::available_moves(Board & board, const Move &) {
    std::list<Move> moves;

    bool end = false;
    int x_move = x, y_move = y;
    while (not end and board.can_step_on(++x_move, y_move, color, end))
        moves.emplace_back(x, y, x_move, y_move);

    end = false;
    x_move = x, y_move = y;
    while (not end and board.can_step_on(--x_move, y_move, color, end))
        moves.emplace_back(x, y, x_move, y_move);

    end = false;
    x_move = x, y_move = y;
    while (not end and board.can_step_on(x_move, ++y_move, color, end))
        moves.emplace_back(x, y, x_move, y_move);

    end = false;
    x_move = x, y_move = y;
    while (not end and board.can_step_on(x_move, --y_move, color, end))
        moves.emplace_back(x, y, x_move, y_move);

    return moves;
}

std::shared_ptr<Piece> Rook::clone() const {
    return std::make_shared<Rook>(*this);
}

std::ostream & Rook::print(std::ostream & out, bool pretty) const {
    pretty ? (color == Color::WHITE ? out << WHITE_ROOK_PRETTY : out << BLACK_ROOK_PRETTY)
           : (color == Color::WHITE ? out << WHITE_ROOK : out << BLACK_ROOK);
    return out;
}
