#include "Pawn.h"

const char * WHITE_PAWN_PRETTY = "\u2659";
const char WHITE_PAWN = 'P';
const char * BLACK_PAWN_PRETTY = "\u265F";
const char BLACK_PAWN = 'p';


Pawn::Pawn(int x, int y, Color color) : Piece(x, y, color) {}

std::list<Move> Pawn::available_moves(Board & board, const Move & last_move) {
    std::list<Move> moves;
    color == Color::WHITE ? moves = white_moves(board, last_move) : moves = black_moves(board, last_move);
    return moves;
}

std::ostream & Pawn::print(std::ostream & out, bool pretty) const {
    pretty ? (color == Color::WHITE ? out << WHITE_PAWN_PRETTY : out << BLACK_PAWN_PRETTY)
           : (color == Color::WHITE ? out << WHITE_PAWN : out << BLACK_PAWN);
    return out;
}

std::list<Move> Pawn::white_moves(const Board & board, const Move & last_move) {
    std::list<Move> moves;

    // en passant (right)
    if (x == 3 and board.is_square_empty(x - 1, y + 1) and last_move.match_coords(1, y + 1, 3, y + 1) and
        board[3][y + 1]->type() == PieceType::PAWN)
        moves.emplace_back(x, y, x - 1, y + 1, false, false, true);

    // en passant (left)
    if (x == 3 and board.is_square_empty(x - 1, y - 1) and last_move.match_coords(1, y - 1, 3, y - 1) and
        board[3][y - 1]->type() == PieceType::PAWN)
        moves.emplace_back(x, y, x - 1, y - 1, false, false, true);

    moves.splice(moves.end(), generate_moves(board, std::minus<>()));
    return moves;
}

std::list<Move> Pawn::black_moves(const Board & board, const Move & last_move) {
    std::list<Move> moves;

    // en passant (right)
    if (x == 4 and board.is_square_empty(x + 1, y + 1) and last_move.match_coords(6, y + 1, 4, y + 1) and
        board[4][y + 1]->type() == PieceType::PAWN)
        moves.emplace_back(x, y, x + 1, y + 1, false, false, true);

    // en passant (left)
    if (x == 4 and board.is_square_empty(x + 1, y - 1) and last_move.match_coords(6, y - 1, 4, y - 1) and
        board[4][y - 1]->type() == PieceType::PAWN)
        moves.emplace_back(x, y, x + 1, y - 1, false, false, true);

    moves.splice(moves.end(), generate_moves(board, std::plus<>()));
    return moves;
}

template<typename Op>
std::list<Move> Pawn::generate_moves(const Board & board, const Op & operation) {
    std::list<Move> moves;

    // promotion cannot be done if pawn has not moved yet (is at initial position)
    // and (x == 1 or x == 6)
    if (not has_moved and board.is_square_empty(operation(x, 1), y) and board.is_square_empty(operation(x, 2), y))
        moves.emplace_back(x, y, operation(x, 2), y);

    if (board.is_square_empty(operation(x, 1), y)) {
        if (operation(x, 1) == 0 or operation(x, 1) == 7) // promotion
            moves.emplace_back(x, y, operation(x, 1), y, true, false, false);
        else
            moves.emplace_back(x, y, operation(x, 1), y);
    }

    // capture moves

    if (board.is_enemy_square(operation(x, 1), y + 1, color)) {
        if (operation(x, 1) == 0 or operation(x, 1) == 7) // promotion
            moves.emplace_back(x, y, operation(x, 1), y + 1, true, false, false);
        else
            moves.emplace_back(x, y, operation(x, 1), y + 1);
    }

    if (board.is_enemy_square(operation(x, 1), y - 1, color)) {
        if (operation(x, 1) == 0 or operation(x, 1) == 7) // promotion
            moves.emplace_back(x, y, operation(x, 1), y - 1, true, false, false);
        else
            moves.emplace_back(x, y, operation(x, 1), y - 1);
    }

    return moves;
}

PieceType Pawn::type() const {
    return PieceType::PAWN;
}

std::shared_ptr<Piece> Pawn::clone() const {
    return std::make_shared<Pawn>(*this);
}
