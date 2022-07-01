#include "Player.h"

#include <utility>

Player::Player(std::list<std::shared_ptr<Piece>> pieces) : pieces(std::move(pieces)) {}

const std::list<std::shared_ptr<Piece>> & Player::get_pieces() const {
    return pieces;
}

std::list<std::shared_ptr<Piece>> & Player::get_pieces() {
    return pieces;
}

bool Player::is_last_move(Board & board, Move & last_move, std::list<std::shared_ptr<Piece>> & enemy_pieces,
                          bool & check, bool & checkmate, bool & stalemate) {
    if (board.is_check(pieces, last_move)) {
        if (board.is_checkmate(enemy_pieces, pieces, last_move)) {
            checkmate = true;
            return true;
        }
        check = true;
    } else {
        if (board.is_checkmate(enemy_pieces, pieces, last_move)) {
            stalemate = true;
            return true;
        }
        check = false;
    }
    return false;
}
