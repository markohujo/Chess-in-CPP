#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"

const char * WHITE_QUEEN_PRETTY = "\u2655";
const char WHITE_QUEEN = 'Q';
const char * BLACK_QUEEN_PRETTY = "\u265B";
const char BLACK_QUEEN = 'q';

Queen::Queen(int x, int y, Color color) : Piece(x, y, color) {}

std::list<Move> Queen::available_moves(Board & board, const Move & last_move) {
    // queen moves like both rook and bishop which is already implemented
    auto moves = Rook(x, y, color).available_moves(board, last_move);
    moves.splice(moves.end(), Bishop(x, y, color).available_moves(board, last_move));
    return moves;
}

PieceType Queen::type() const {
    return PieceType::QUEEN;
}

std::shared_ptr<Piece> Queen::clone() const {
    return std::make_shared<Queen>(*this);
}

std::ostream & Queen::print(std::ostream & out, bool pretty) const {
    pretty ? (color == Color::WHITE ? out << WHITE_QUEEN_PRETTY : out << BLACK_QUEEN_PRETTY)
           : (color == Color::WHITE ? out << WHITE_QUEEN : out << BLACK_QUEEN);
    return out;
}
