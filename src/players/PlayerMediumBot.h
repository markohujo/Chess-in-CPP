#pragma once

#include "PlayerBot.h"

class PlayerMediumBot : public PlayerBot {
public:
    explicit PlayerMediumBot(const std::list<std::shared_ptr<Piece>> & pieces);

    /**
     * Randomly chooses one of the highest valued available and legal moves which means it does capture a piece if possible
     * Uses piece relative position evaluation function in Board::calcValue method
     *
     * @return true if game continues after this move
     * @return false if game ends after this move (this move caused checkmate or stalemate)
     */
    bool take_turn(const Interface & interface, Board & board, Move & last_move,
                   std::list<std::shared_ptr<Piece>> & enemy_pieces,
                   bool & check, bool & checkmate, bool & stalemate) override;
};
