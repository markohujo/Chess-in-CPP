#pragma once

#include "Board.h"
#include "enums/PieceType.h"

class Board;

class Piece;

enum class Color;

class Move {
public:
    Move() = default;

    Move(int x_from, int y_from, int x_to, int y_to, bool promotion = false, bool castling = false,
         bool en_passant = false);

    /**
     * Executes this move (moves piece, captures, castles, etc.)
     */
    void execute(Board & board, std::list<std::shared_ptr<Piece>> & player_pieces);

    /**
     * Undoes this move after it was executed
     */
    void undo(Board & board, std::list<std::shared_ptr<Piece>> & player_pieces);

    /**
     * Compares the given destination coordinates to this move's destination coordinates
     * @return true if destination coordinates match, false otherwise
     */
    bool match_dest_coords(int x, int y) const;

    /**
     * Compares the given source and destination coordinates to this move's coordinates
     * @param x_from_p source x
     * @param y_from_p source y
     * @param x_to_p destination x
     * @param y_to_p destination y
     * @return true if coordinates match, false otherwise
     */
    bool match_coords(int x_from_p, int y_from_p, int x_to_p, int y_to_p) const;

    int get_x_to() const;

    int get_y_to() const;

    int get_x_from() const;

    int get_y_from() const;

    bool is_promotion() const;

    void set_new_promotion_piece(const std::string & piece_str, Color color);

    bool is_castling() const;

private:
    int x_from = 0, y_from = 0, x_to = 0, y_to = 0;

    std::shared_ptr<Piece> capture_piece = nullptr;

    bool first_move = false;

    bool promotion = false;

    bool castling = false;

    bool en_passant = false;

    std::shared_ptr<Piece> promoted = nullptr;

    std::shared_ptr<Piece> new_promotion_piece = nullptr;
};
