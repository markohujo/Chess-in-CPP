#pragma once

#include "PlayerBot.h"

class PlayerEasyBot : public PlayerBot {
public:
    explicit PlayerEasyBot(const std::list<std::shared_ptr<Piece>> & pieces);

    /**
     * Randomly chooses one of the available and legal moves
     * @return true if game continues after this move
     * @return false if game ends after this move (this move caused checkmate or stalemate)
     */
    bool take_turn(const Interface & interface, Board & board, Move & last_move,
                   std::list<std::shared_ptr<Piece>> & enemy_pieces,
                   bool & check, bool & checkmate, bool & stalemate) override;

private:
    /**
     * @return randomly generated available move
     * @warning Move might be invalid (might get this player in check)
     */
    Move generate_random_move(Board & board, const Move & last_move) const;
};
