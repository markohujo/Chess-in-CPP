#include "Bishop.h"

const char * WHITE_BISHOP_PRETTY = "\u2657";
const char WHITE_BISHOP = 'B';
const char * BLACK_BISHOP_PRETTY = "\u265D";
const char BLACK_BISHOP = 'b';

Bishop::Bishop(int x, int y, Color color) : Piece(x, y, color) {}

std::list<Move> Bishop::available_moves(Board & board, const Move &) {
    std::list<Move> moves;

    bool end = false;
    int x_move = x, y_move = y;
    while (not end and board.can_step_on(++x_move, ++y_move, color, end))
        moves.emplace_back(x, y, x_move, y_move);

    end = false;
    x_move = x, y_move = y;
    while (not end and board.can_step_on(--x_move, --y_move, color, end))
        moves.emplace_back(x, y, x_move, y_move);

    end = false;
    x_move = x, y_move = y;
    while (not end and board.can_step_on(++x_move, --y_move, color, end))
        moves.emplace_back(x, y, x_move, y_move);

    end = false;
    x_move = x, y_move = y;
    while (not end and board.can_step_on(--x_move, ++y_move, color, end))
        moves.emplace_back(x, y, x_move, y_move);

    return moves;
}

std::ostream & Bishop::print(std::ostream & out, bool pretty) const {
    pretty ? (color == Color::WHITE ? out << WHITE_BISHOP_PRETTY : out << BLACK_BISHOP_PRETTY)
           : (color == Color::WHITE ? out << WHITE_BISHOP : out << BLACK_BISHOP);
    return out;
}

PieceType Bishop::type() const {
    return PieceType::BISHOP;
}

std::shared_ptr<Piece> Bishop::clone() const {
    return std::make_shared<Bishop>(*this);
}
