#include <queue>
#include <climits>
#include "PlayerHardBot.h"

const int MINIMAX_DEPTH = 3;

PlayerHardBot::PlayerHardBot(const std::list<std::shared_ptr<Piece>> & pieces) : PlayerBot(pieces) {}

bool
PlayerHardBot::take_turn(const Interface & interface, Board & board, Move & last_move,
                         std::list<std::shared_ptr<Piece>> & enemy_pieces, bool & check, bool & checkmate,
                         bool & stalemate) {
    interface.print_string("I'm calculating my move ...");
    Move move = find_best_move(board, last_move, enemy_pieces);
    move.execute(board, pieces);
    last_move = move;
    return not is_last_move(board, last_move, enemy_pieces, check, checkmate, stalemate);
}

Move
PlayerHardBot::find_best_move(Board & board, const Move & last_move, std::list<std::shared_ptr<Piece>> & enemy_pieces) {
    int best_value = INT_MIN;
    Move best_move;

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

            // this is the first level of the tree as we go through all bot's possible moves which is the maximizing player ...
            // ... so the next player is minimizing player, that's why minimax is called with is_max param set to false
            int value = minimax(INT_MIN, INT_MAX, MINIMAX_DEPTH, false, board, move, enemy_pieces, pieces);
            move.undo(board, pieces);

            // choose the best valued move as this is the maximizing player
            if (value > best_value) {
                best_value = value;
                best_move = move;
            }
        }
    }

    return best_move;
}

int PlayerHardBot::minimax(int alpha, int beta, int depth, bool is_max, Board & board, const Move & last_move,
                           std::list<std::shared_ptr<Piece>> & player_pieces,
                           std::list<std::shared_ptr<Piece>> & enemy_pieces) {
    if (depth == 0)
        return board.calc_value(Color::BLACK);

    int value;

    // if maximizing player, find move with the highest value
    if (is_max) {

        // for all pieces and all their moves
        for (const auto & piece: player_pieces) {

            // skip this piece if it's been captured or promoted
            if (not piece->is_available())
                continue;

            for (auto & move: piece->available_moves(board, last_move)) {
                move.execute(board, player_pieces);
                value = minimax(alpha, beta, depth - 1, false, board, move, enemy_pieces, player_pieces);
                move.undo(board, player_pieces);

                if (value >= beta)
                    return beta;
                alpha = std::max(alpha, value);

            }
        }

        return alpha;
    }

    // if minimizing player, find move with the lowest value
    // else is redundant, but it's prettier
    else {

        // for all pieces and all their moves
        for (const auto & piece: player_pieces) {

            // skip this piece if it's been captured or promoted
            if (not piece->is_available())
                continue;

            for (auto & move: piece->available_moves(board, last_move)) {
                move.execute(board, player_pieces);
                value = minimax(alpha, beta, depth - 1, true, board, move, enemy_pieces, player_pieces);
                move.undo(board, player_pieces);

                if (value <= alpha)
                    return alpha;
                beta = std::min(beta, value);

            }
        }

        return beta;
    }
}



