#include <random>
#include <climits>
#include "PlayerMediumBot.h"

PlayerMediumBot::PlayerMediumBot(const std::list<std::shared_ptr<Piece>> & pieces) : PlayerBot(pieces) {}

bool
PlayerMediumBot::take_turn(const Interface & interface, Board & board, Move & last_move,
                           std::list<std::shared_ptr<Piece>> & enemy_pieces, bool & check, bool & checkmate,
                           bool & stalemate) {
    interface.print_string("I'm calculating my move ...");
    std::vector<Move> best_moves;
    int best_value = INT_MIN;

    for (const auto & piece: pieces) {

        // skip this piece if it's been captured or promoted
        if (not piece->is_available())
            continue;

        for (auto & move: piece->available_moves(board, last_move)) {

            // cannot play castling if in check
            if (move.is_castling() and board.is_check(enemy_pieces, last_move))
                continue;

            move.execute(board, pieces);

            // skip this move if it would result in this player's check (illegal move)
            if (board.is_check(enemy_pieces, move)) {
                move.undo(board, pieces);
                continue;
            }

            int val = board.calc_value(pieces.front()->get_color());

            if (val == best_value)
                best_moves.push_back(move);

            else if (val > best_value) {
                best_value = val;
                best_moves.clear();
                best_moves.push_back(move);
            }

            move.undo(board, pieces);
        }
    }

    // randomly choose one of the highest valued moves
    std::shuffle(best_moves.begin(), best_moves.end(), std::mt19937(std::random_device()()));
    Move move = best_moves[0];
    move.execute(board, pieces);
    last_move = move;

    return not is_last_move(board, last_move, enemy_pieces, check, checkmate, stalemate);
}
