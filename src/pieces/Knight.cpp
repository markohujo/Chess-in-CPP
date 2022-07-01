#include "Knight.h"

const char * WHITE_KNIGHT_PRETTY = "\u2658";
const char WHITE_KNIGHT = 'N';
const char * BLACK_KNIGHT_PRETTY = "\u265E";
const char BLACK_KNIGHT = 'n';

Knight::Knight(int x, int y, Color color) : Piece(x, y, color) {}

std::list<Move> Knight::available_moves(Board & board, const Move &) {
    std::list<Move> moves;

    if (board.can_step_on(x + 2, y + 1, color))
        moves.emplace_back(x, y, x + 2, y + 1);

    if (board.can_step_on(x + 2, y - 1, color))
        moves.emplace_back(x, y, x + 2, y - 1);

    if (board.can_step_on(x + 1, y + 2, color))
        moves.emplace_back(x, y, x + 1, y + 2);

    if (board.can_step_on(x + 1, y - 2, color))
        moves.emplace_back(x, y, x + 1, y - 2);

    if (board.can_step_on(x - 2, y + 1, color))
        moves.emplace_back(x, y, x - 2, y + 1);

    if (board.can_step_on(x - 2, y - 1, color))
        moves.emplace_back(x, y, x - 2, y - 1);

    if (board.can_step_on(x - 1, y + 2, color))
        moves.emplace_back(x, y, x - 1, y + 2);

    if (board.can_step_on(x - 1, y - 2, color))
        moves.emplace_back(x, y, x - 1, y - 2);

    return moves;
}

PieceType Knight::type() const {
    return PieceType::KNIGHT;
}

std::shared_ptr<Piece> Knight::clone() const {
    return std::make_shared<Knight>(*this);
}

std::ostream & Knight::print(std::ostream & out, bool pretty) const {
    pretty ? (color == Color::WHITE ? out << WHITE_KNIGHT_PRETTY : out << BLACK_KNIGHT_PRETTY)
           : (color == Color::WHITE ? out << WHITE_KNIGHT : out << BLACK_KNIGHT);
    return out;
}
