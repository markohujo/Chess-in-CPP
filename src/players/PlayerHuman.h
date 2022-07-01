#pragma once


#include <unordered_map>
#include "Player.h"

class PlayerHuman : public Player {
public:
    explicit PlayerHuman(std::list<std::shared_ptr<Piece>> pieces);

    /**
     * Loads user input (source and destination coordinates of the move), validates them and executes move
     * @return true if move was successfully executed and game continues after this move
     * @return false if game ends after this move (this move caused checkmate or stalemate) or if user input is invalid
     */
    virtual bool take_turn(const Interface & interface, Board & board, Move & last_move,
                           std::list<std::shared_ptr<Piece>> & enemy_pieces, bool & check, bool & checkmate,
                           bool & stalemate) override;

private:
    /**
     * TODO
     * @param coords_str
     * @param piece
     * @return
     */
    bool has_piece(const std::string & coords_str, std::shared_ptr<Piece> & piece);

    /**
     * TODO
     * @param coords_str
     * @param moves
     * @param move
     * @return
     */
    bool can_move(const std::string & coords_str, const std::list<Move> & moves, Move & move);

    std::unordered_map<char, int> coords_map = {
            {'a', 0},
            {'b', 1},
            {'c', 2},
            {'d', 3},
            {'e', 4},
            {'f', 5},
            {'g', 6},
            {'h', 7},
            {'1', 7},
            {'2', 6},
            {'3', 5},
            {'4', 4},
            {'5', 3},
            {'6', 2},
            {'7', 1},
            {'8', 0}
    };
};
