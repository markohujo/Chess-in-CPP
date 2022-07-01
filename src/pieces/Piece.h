#pragma once

#include <list>
#include <ostream>
#include "../Move.h"
#include "../enums/Color.h"
#include "../Board.h"

class Board;

class Move;

enum class PieceType;

class Piece {
public:
    Piece(int x, int y, Color color);

    virtual ~Piece() = default;

    /**
     * Generates all available moves for this piece (depends on its type and coordinates)
     * @return list containing all available moves
     * @warning illegal moves that result in player's check are also included in the returned list
     */
    virtual std::list<Move> available_moves(Board & board, const Move &) = 0;

    virtual PieceType type() const = 0;

    virtual std::shared_ptr<Piece> clone() const = 0;

    bool match_coords(int x_p, int y_p) const;

    bool match_color(Color color_p) const;

    /**
     * Updates coordinates of this piece
     * @param[out] first_move sets to true if this is this piece's first move
     */
    void move(int x_new, int y_new, bool & first_move);

    /**
     * Undoes the last move this piece has made
     * @param[in] undo_has_moved determines whether has_moved should be set back to false
     */
    void undo_move(int x_new, int y_new, bool undo_has_moved);

    /**
     * Sets captured field to true and coordinates to -1,-1
     */
    void capture();

    /**
     * Sets coordinates and captured field to false
     */
    void un_capture(int x_p, int y_p);

    /**
     * @return true if this piece is available (has not been captured or promoted)
     */
    bool is_available() const;

    bool was_captured() const;

    /**
     * @return true if this piece has already made its first move
     * @return false if this has not moved yet
     */
    bool has_already_moved() const;

    void change_availability(bool a, int x_p = -1, int y_p = -1);

    void set_has_moved(bool moved);

    Color get_color() const;

    int get_x() const;

    int get_y() const;

    virtual std::ostream & print(std::ostream & out, bool pretty) const = 0;

    friend std::ostream & operator<<(std::ostream & out, const Piece & piece);

protected:
    int x, y;

    Color color;

    bool has_moved = false;

    bool available = true;

    bool captured = false;
};
