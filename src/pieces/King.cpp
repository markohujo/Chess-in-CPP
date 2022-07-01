#include <iostream>
#include "King.h"

const char * WHITE_KING_PRETTY = "\u2654";
const char WHITE_KING = 'K';
const char * BLACK_KING_PRETTY = "\u265A";
const char BLACK_KING = 'k';

King::King(int x, int y, Color color) : Piece(x, y, color) {}

std::list<Move> King::available_moves(Board & board, const Move &) {
    std::list<Move> moves;

    if (board.can_step_on(x, y + 1, color))
        moves.emplace_back(x, y, x, y + 1);

    if (board.can_step_on(x + 1, y + 1, color))
        moves.emplace_back(x, y, x + 1, y + 1);

    if (board.can_step_on(x + 1, y, color))
        moves.emplace_back(x, y, x + 1, y);

    if (board.can_step_on(x + 1, y - 1, color))
        moves.emplace_back(x, y, x + 1, y - 1);

    if (board.can_step_on(x, y - 1, color))
        moves.emplace_back(x, y, x, y - 1);

    if (board.can_step_on(x - 1, y - 1, color))
        moves.emplace_back(x, y, x - 1, y - 1);

    if (board.can_step_on(x - 1, y, color))
        moves.emplace_back(x, y, x - 1, y);

    if (board.can_step_on(x - 1, y + 1, color))
        moves.emplace_back(x, y, x - 1, y + 1);

    // castling short (mala rosada)
    if (not has_moved and board.is_square_empty(x, y + 1) and board.is_square_empty(x, y + 2) and
        board[x][y + 3] != nullptr and board[x][y + 3]->type() == PieceType::ROOK and
        not board[x][y + 3]->has_already_moved())
        moves.emplace_back(x, y, x, y + 2, false, true, false);

    // castling long (velka rosada)
    if (not has_moved and board.is_square_empty(x, y - 1) and board.is_square_empty(x, y - 2) and
        board.is_square_empty(x, y - 3) and board[x][y - 4] != nullptr and
        board[x][y - 4]->type() == PieceType::ROOK and not board[x][y - 4]->has_already_moved())
        moves.emplace_back(x, y, x, y - 2, false, true, false);

    return moves;
}

std::shared_ptr<Piece> King::clone() const {
    return std::make_shared<King>(*this);
}

std::ostream & King::print(std::ostream & out, bool pretty) const {
    pretty ? (color == Color::WHITE ? out << WHITE_KING_PRETTY : out << BLACK_KING_PRETTY)
           : (color == Color::WHITE ? out << WHITE_KING : out << BLACK_KING);
    return out;
}

PieceType King::type() const {
    return PieceType::KING;
}
