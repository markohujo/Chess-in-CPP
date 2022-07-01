#include <random>
#include "PlayerEasyBot.h"

PlayerEasyBot::PlayerEasyBot(const std::list<std::shared_ptr<Piece>> & pieces) : PlayerBot(pieces) {}

bool PlayerEasyBot::take_turn(const Interface & interface, Board & board, Move & last_move,
                              std::list<std::shared_ptr<Piece>> & enemy_pieces, bool & check, bool & checkmate,
                              bool & stalemate) {
    interface.print_string("I'm calculating my move ...");

    Move move = generate_random_move(board, last_move);

    // cannot play castling if in check
    while (move.is_castling() and board.is_check(enemy_pieces, last_move))
        move = generate_random_move(board, last_move);

    move.execute(board, pieces);

    // skip all illegal moves that would result in this player's check
    while (board.is_check(enemy_pieces, move)) {
        move.undo(board, pieces);
        move = generate_random_move(board, last_move);
        move.execute(board, pieces);
    }
    last_move = move;

    return not is_last_move(board, last_move, enemy_pieces, check, checkmate, stalemate);
}

Move PlayerEasyBot::generate_random_move(Board & board, const Move & last_move) const {
    std::list<Move> all_moves;

    for (const auto & piece: pieces) {
        // skip this piece if it's been captured or promoted
        if (not piece->is_available())
            continue;
        all_moves.splice(all_moves.begin(), piece->available_moves(board, last_move));
    }

    // moves must be copied to vector as list cannot be shuffled
    std::vector<Move> all_moves_v(all_moves.begin(), all_moves.end());
    std::shuffle(all_moves_v.begin(), all_moves_v.end(), std::mt19937(std::random_device()()));
    return all_moves_v[0];
}
