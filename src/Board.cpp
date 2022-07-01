#include "Board.h"
#include "pieces/Pawn.h"
#include <iostream>
#include <map>
#include <vector>

#ifndef NOTPRETTY
const bool PRETTY_PRINT = true;
#else
const bool PRETTY_PRINT = false;
#endif

const char * WHITE_EMPTY_PRETTY = "\u25FB";
const char * BLACK_EMPTY_PRETTY = "\u25FC";
const char EMPTY = '-';

Board::Board(const std::list<std::shared_ptr<Piece>> & white_pieces,
             const std::list<std::shared_ptr<Piece>> & black_pieces) {

    size_t row_i = 0, column_i = 0;
    for (const auto & piece: black_pieces) {
        board[row_i][column_i++] = piece;
        if (column_i == 8) {
            row_i++;
            column_i = 0;
        }
    }

    row_i = 6, column_i = 0;
    for (const auto & piece: white_pieces) {
        board[row_i][column_i++] = piece;
        if (column_i == 8) {
            row_i++;
            column_i = 0;
        }
    }
}

std::ostream & operator<<(std::ostream & out, const Board & board) {
    for (const auto & row: board.board) {
        for (const auto & square: row) {
            if (square == nullptr)
                out << '-';
            else
                square->print(out, false);
        }
        out << std::endl;
    }
    return out;
}

void Board::print(std::ostream & out) const {
    out << "     A B C D E F G H" << std::endl;
    out << "   +-----------------+" << std::endl;
    size_t row_cnt = 8;
    for (const auto & row: board) {
        out << " " << row_cnt << " | ";
        size_t column_cnt = 8;
        for (const auto & square: row) {
            if (square == nullptr)
                print_empty((row_cnt + column_cnt) % 2, PRETTY_PRINT, out);
            else
                square->print(out, PRETTY_PRINT);
            out << " ";
            --column_cnt;
        }
        out << "| " << row_cnt-- << std::endl;
    }
    out << "   +-----------------+" << std::endl;
    out << "     A B C D E F G H" << std::endl << std::endl;
}

const std::array<std::shared_ptr<Piece>, 8> & Board::operator[](size_t i) const {
    if (i > 7)
        throw std::invalid_argument("Out of bounds.");
    return board[i];
}

std::array<std::shared_ptr<Piece>, 8> & Board::operator[](size_t i) {
    if (i > 7)
        throw std::invalid_argument("Out of bounds.");
    return board[i];
}

bool Board::is_square_empty(int x, int y) const {
    if (not valid_coords(x, y))
        return false;
    return board[x][y] == nullptr;
}

bool Board::is_enemy_square(int x, int y, Color color) const {
    if (not valid_coords(x, y))
        return false;
    if (board[x][y] == nullptr)
        return false;
    return board[x][y]->match_color(!color);
}

bool Board::can_step_on(int x, int y, Color color) const {
    if (not valid_coords(x, y))
        return false;
    return is_square_empty(x, y) or is_enemy_square(x, y, color);
}

bool Board::can_step_on(int x, int y, Color color, bool & occupied) const {
    if (not valid_coords(x, y))
        return false;
    if (not is_square_empty(x, y))
        occupied = true;
    return is_square_empty(x, y) or is_enemy_square(x, y, color);
}

bool Board::valid_coords(int x, int y) {
    if (x < 0 or x > 7)
        return false;
    if (y < 0 or y > 7)
        return false;
    return true;
}

bool Board::is_check(const std::list<std::shared_ptr<Piece>> & enemy_pieces, const Move & last_move) {
    for (const auto & piece: enemy_pieces) {
        if (not piece->is_available())
            continue;
        for (const auto & move: piece->available_moves(*this, last_move)) {
            if (board[move.get_x_to()][move.get_y_to()] != nullptr and
                board[move.get_x_to()][move.get_y_to()]->type() == PieceType::KING)
                return true;
        }
    }
    return false;
}

bool
Board::is_checkmate(std::list<std::shared_ptr<Piece>> & pieces, const std::list<std::shared_ptr<Piece>> & enemy_pieces,
                    const Move & last_move) {
    for (const auto & piece: pieces) {
        if (not piece->is_available())
            continue;
        for (auto & move: piece->available_moves(*this, last_move)) {
            move.execute(*this, pieces);
            if (not is_check(enemy_pieces, move)) {
                move.undo(*this, pieces);
                return false;
            }
            move.undo(*this, pieces);
        }
    }
    return true;
}

int Board::calc_value(Color color) const {
    // source: https://en.wikipedia.org/wiki/Chess_piece_relative_value
    std::map<PieceType, int> pieces_values = {
            {PieceType::PAWN,   100},
            {PieceType::KNIGHT, 305},
            {PieceType::BISHOP, 333},
            {PieceType::ROOK,   563},
            {PieceType::QUEEN,  950},
            {PieceType::KING,   10000}
    };

    std::vector<std::vector<int>> pawn_values = {
            {0,  0,  0,   0,   0,   0,   0,  0},
            {50, 50, 50,  50,  50,  50,  50, 50},
            {10, 10, 20,  30,  30,  20,  10, 1},
            {5,  5,  10,  25,  25,  10,  5,  5},
            {0,  0,  0,   20,  20,  0,   0,  0},
            {5,  -5, -10, 0,   0,   -10, -5, 5},
            {5,  10, 10,  -20, -20, 10,  10, 5},
            {0,  0,  0,   0,   0,   0,   0,  0}
    };

    std::vector<std::vector<int>> knight_values = {
            {-50, -40, -30, -30, -30, -30, -40, -50},
            {-40, -20, 0,   0,   0,   0,   -20, -40},
            {-30, 0,   10,  15,  15,  10,  0,   -30},
            {-30, 5,   15,  20,  20,  15,  5,   -30},
            {-30, 0,   15,  20,  20,  15,  0,   -30},
            {-30, 5,   10,  15,  15,  10,  5,   -30},
            {-40, -20, 0,   5,   5,   0,   -20, -40},
            {-50, -40, -30, -30, -30, -30, -40, -50}
    };

    std::vector<std::vector<int>> bishop_values = {
            {-20, -10, -10, -10, -10, -10, -10, -20},
            {-10, 0,   0,   0,   0,   0,   0,   -10},
            {-10, 0,   5,   10,  10,  5,   0,   -10},
            {-10, 5,   5,   10,  10,  5,   5,   -10},
            {-10, 0,   10,  10,  10,  10,  0,   -10},
            {-10, 10,  10,  10,  10,  10,  10,  -10},
            {-10, 5,   0,   0,   0,   0,   5,   -10},
            {-20, -10, -10, -10, -10, -10, -10, -20},
    };

    std::vector<std::vector<int>> rook_values = {
            {0,  0,  0,  0,  0,  0,  0,  0},
            {5,  10, 10, 10, 10, 10, 10, 5},
            {-5, 0,  0,  0,  0,  0,  0,  -5},
            {-5, 0,  0,  0,  0,  0,  0,  -5},
            {-5, 0,  0,  0,  0,  0,  0,  -5},
            {-5, 0,  0,  0,  0,  0,  0,  -5},
            {-5, 0,  0,  0,  0,  0,  0,  -5},
            {0,  0,  0,  5,  5,  0,  0,  0}
    };

    std::vector<std::vector<int>> queen_values = {
            {-20, -10, -10, -5, -5, -10, -10, -20},
            {-10, 0,   0,   0,  0,  0,   0,   -10},
            {-10, 0,   5,   5,  5,  5,   0,   -10},
            {-5,  0,   5,   5,  5,  5,   0,   -5},
            {0,   0,   5,   5,  5,  5,   0,   -5},
            {-10, 5,   5,   5,  5,  5,   0,   -10},
            {-10, 0,   5,   0,  0,  0,   0,   -10},
            {-20, -10, -10, -5, -5, -10, -10, -20}
    };

    std::vector<std::vector<int>> king_values{
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-20, -30, -30, -40, -40, -30, -30, -20},
            {-10, -20, -20, -20, -20, -20, -20, -10},
            {20,  20,  0,   0,   0,   0,   20,  20},
            {20,  30,  10,  0,   0,   10,  30,  20}
    };

    int value = 0;
    for (const auto & row: board) {
        for (const auto & square: row) {
            if (square == nullptr)
                continue;

            // get piece's value
            int piece_value = pieces_values[square->type()];

            // add/subtract piece's value depending on where it is located
            switch (square->type()) {
                case PieceType::PAWN:
                    square->get_color() == Color::WHITE ? piece_value += pawn_values[square->get_x()][square->get_y()]
                                                        : piece_value += pawn_values[7 - square->get_x()][7 -
                                                                                                          square->get_y()];
                    break;

                case PieceType::ROOK:
                    square->get_color() == Color::WHITE ? piece_value += rook_values[square->get_x()][square->get_y()]
                                                        : piece_value += rook_values[7 - square->get_x()][7 -
                                                                                                          square->get_y()];
                    break;

                case PieceType::KNIGHT:
                    square->get_color() == Color::WHITE ? piece_value += knight_values[square->get_x()][square->get_y()]
                                                        : piece_value += knight_values[7 - square->get_x()][7 -
                                                                                                            square->get_y()];
                    break;

                case PieceType::BISHOP:
                    square->get_color() == Color::WHITE ? piece_value += bishop_values[square->get_x()][square->get_y()]
                                                        : piece_value += bishop_values[7 - square->get_x()][7 -
                                                                                                            square->get_y()];
                    break;

                case PieceType::QUEEN:
                    square->get_color() == Color::WHITE ? piece_value += queen_values[square->get_x()][square->get_y()]
                                                        : piece_value += queen_values[7 - square->get_x()][7 -
                                                                                                           square->get_y()];
                    break;

                case PieceType::KING:
                    square->get_color() == Color::WHITE ? piece_value += king_values[square->get_x()][square->get_y()]
                                                        : piece_value += king_values[7 - square->get_x()][7 -
                                                                                                          square->get_y()];
                    break;

                default:
                    break;
            }

            // add piece's value if piece is in the given color, subtract it otherwise
            square->get_color() == color ? value += piece_value : value -= piece_value;
        }
    }
    return value;
}

void Board::print_empty(bool white, bool pretty, std::ostream & out) {
    pretty ? (white ? out << WHITE_EMPTY_PRETTY : out << BLACK_EMPTY_PRETTY)
           : out << EMPTY;
}
