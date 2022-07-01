#pragma once

#include <array>
#include <memory>
#include "pieces/Piece.h"

class Piece;

class Move;

enum class Color;

/**
 * Represents a chessboard that is 8x8 squares that might contain a chess piece
 */
class Board {
public:
    Board() = default;

    /** Constructs the board at the beginning of the game according to the rules of chess */
    Board(const std::list<std::shared_ptr<Piece>> & white_pieces,
          const std::list<std::shared_ptr<Piece>> & black_pieces);

    /**
     * @return true if square with the given coordinates x and y is empty (no piece is located at the given square)
     * @return false if the given square is occupied by a piece
     * @return false if the given coordinates are out of range (not in {0, 1, ..., 6, 7})
     */
    bool is_square_empty(int x, int y) const;

    /**
     * @return true if square with the given coordinates x and y is occupied by an enemy piece (piece in the opposite color of the given color)
     * @return false otherwise
     * @return false if the given coordinates are out of range (not in {0, 1, ..., 6, 7})
     */
    bool is_enemy_square(int x, int y, Color color) const;

    /**
     * @return true if square with the given coordinates x and y is either empty or occupied by an enemy piece (piece in the opposite color of the given color)
     * @return false otherwise
     * @return false if the given coordinates are out of range (not in {0, 1, ..., 6, 7})
     */
    bool can_step_on(int x, int y, Color color) const;

    /**
     * Same as can_step_on above, but sets occupied to true if the given square is occupied
     * @param[out] occupied sets occupied to true if square is occupied
     * @return true if square with the given coordinates x and y is either empty or occupied by an enemy piece (piece in the opposite color of the given color)
     * @return false otherwise
     * @return false if the given coordinates are out of range (not in {0, 1, ..., 6, 7})     */
    bool can_step_on(int x, int y, Color color, bool & occupied) const;

    /**
     * Calculates this board's current value for the given color based on what type of pieces are on the board and where on the board they are located
     * @return calculated value
     */
    int calc_value(Color color) const;

    /**
     * @return true if last move gets player in check
     */
    bool is_check(const std::list<std::shared_ptr<Piece>> & enemy_pieces, const Move & last_move);

    /**
     * @return true if last move gets player in checkmate
     */
    bool is_checkmate(std::list<std::shared_ptr<Piece>> & pieces,
                      const std::list<std::shared_ptr<Piece>> & enemy_pieces, const Move & last_move);

    const std::array<std::shared_ptr<Piece>, 8> & operator[](size_t i) const;

    std::array<std::shared_ptr<Piece>, 8> & operator[](size_t i);

    /** Used for pretty print when showing this board to the user */
    void print(std::ostream & out) const;

    /** Used for outputting this board to file when saving the game */
    friend std::ostream & operator<<(std::ostream & out, const Board & board);

private:
    /** 2D 8x8 array which represents a chessboard */
    std::array<std::array<std::shared_ptr<Piece>, 8>, 8> board;

    /**
     * @return true if the given coordinates are valid (are in range {0, 1, ..., 6, 7})
     * Methods using this method could just throw an exception but I find this better
     */
    static bool valid_coords(int x, int y);

    static void print_empty(bool white, bool pretty, std::ostream & out);
};
