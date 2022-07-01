#pragma once


#include <memory>
#include <list>
#include "../pieces/Piece.h"
#include "../Interface.h"

class Interface;

/**
 * Represents one of the two chess players (can be either human or bot)
 */
class Player {
public:
    explicit Player(std::list<std::shared_ptr<Piece>> pieces);

    virtual ~Player() = default;

    /**
     * Takes turn depending on what type of player this is (human or bot - easy, medium, hard)
     * @return true if move was successfully executed and game continues after this move
     * @return false if game ends after this move (this move caused checkmate or stalemate) or if user input in PlayerHuman is invalid
     */
    virtual bool take_turn(const Interface & interface, Board & board, Move & last_move,
                           std::list<std::shared_ptr<Piece>> & enemy_pieces,
                           bool & check, bool & check_mate, bool & stalemate) = 0;

    const std::list<std::shared_ptr<Piece>> & get_pieces() const;

    std::list<std::shared_ptr<Piece>> & get_pieces();

protected:
    std::list<std::shared_ptr<Piece>> pieces;

    /**
     * Checks whether game comes to end after the last move that means if player is in checkmate or if game is in stalemate (draw)
     * @param[out] check sets check to true if player is in check after the last move
     * @param[out] checkmate sets checkmate to true if player is in checkmate
     * @param[out] stalemate sets stalemate to true if game is in stalemate (draw)
     * @return true if game comes to end after the last move
     * @return false if game still continues after the last move
     */
    bool is_last_move(Board & board, Move & last_move, std::list<std::shared_ptr<Piece>> & enemy_pieces,
                      bool & check, bool & checkmate, bool & stalemate);
};
