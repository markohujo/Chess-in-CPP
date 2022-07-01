#pragma once

#include "PlayerBot.h"

class PlayerHardBot : public PlayerBot {
public:
    explicit PlayerHardBot(const std::list<std::shared_ptr<Piece>> & pieces);

    /**
     * Uses minimax algorithm with alpha-beta pruning and piece relative position evaluation function in Board::calcValue method
     * @return true if game continues after this move
     * @return false if game ends after this move (this move caused checkmate or stalemate)
     */
    bool take_turn(const Interface & interface, Board & board, Move & last_move,
                   std::list<std::shared_ptr<Piece>> & enemy_pieces,
                   bool & check, bool & checkmate, bool & stalemate) override;

private:
    /**
     * Goes through all player's pieces and their moves, starts recursion (calls minimax) and finds the best player's move
     * @return best player's move after minimax search
     */
    Move find_best_move(Board & board, const Move & last_move, std::list<std::shared_ptr<Piece>> & enemy_pieces);

    /**
     * Implements recursive minimax algorithm with alpha-beta pruning
     * @param[in] is_max - if player is maximizing or minimizing
     * @return value - min or max value (depending on is_max) representing value of the best/worst move
     */
    int minimax(int alpha, int beta, int depth, bool is_max, Board & board, const Move & last_move,
                std::list<std::shared_ptr<Piece>> & player_pieces,
                std::list<std::shared_ptr<Piece>> & enemy_pieces);
};
