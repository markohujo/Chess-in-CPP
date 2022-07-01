#include "PlayerHuman.h"
#include "../Interface.h"

PlayerHuman::PlayerHuman(std::list<std::shared_ptr<Piece>> pieces) : Player(std::move(pieces)) {}

bool
PlayerHuman::take_turn(const Interface & interface, Board & board, Move & last_move,
                       std::list<std::shared_ptr<Piece>> & enemy_pieces, bool & check, bool & checkmate,
                       bool & stalemate) {

    std::shared_ptr<Piece> piece;
    std::string coords_str;

    if (not interface.prompt_coords(coords_str))
        return false;

    if (not has_piece(coords_str, piece)) {
        interface.print_string("No piece at " + coords_str.substr(0, 2) + ".");
        return false;
    }

    auto moves = piece->available_moves(board, last_move);

    if (moves.empty()) {
        interface.print_string("Cannot move piece at " + coords_str.substr(0, 2) + ".");
        return false;
    }

    Move move{};
    if (not can_move(coords_str, moves, move)) {
        interface.print_string(
                "Cannot move piece at " + coords_str.substr(0, 2) + " to " + coords_str.substr(2, 2) + ".");
        return false;
    }

    if (move.is_castling() and board.is_check(enemy_pieces, last_move)) {
        interface.print_string("Cannot play castling move if you are in check.");
        return false;
    }

    if (move.is_promotion())
        move.set_new_promotion_piece(interface.prompt_promotion(), pieces.front()->get_color());

    move.execute(board, pieces);
    if (board.is_check(enemy_pieces, move)) {
        move.undo(board, pieces);
        interface.print_string("Cannot move here. Check!");
        return false;
    }
    last_move = move;

    return not is_last_move(board, last_move, enemy_pieces, check, checkmate, stalemate);
}

bool PlayerHuman::has_piece(const std::string & coords_str, std::shared_ptr<Piece> & piece) {
    for (const auto & p: pieces) {
        if (p->is_available() and p->match_coords(coords_map[coords_str[1]], coords_map[coords_str[0]])) {
            piece = p;
            return true;
        }
    }
    return false;
}

bool PlayerHuman::can_move(const std::string & coords_str, const std::list<Move> & moves, Move & move) {
    for (const auto & m: moves) {
        if (m.match_dest_coords(coords_map[coords_str[3]], coords_map[coords_str[2]])) {
            move = m;
            return true;
        }
    }
    return false;
}
