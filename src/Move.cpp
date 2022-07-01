#include <iostream>
#include "Move.h"
#include "pieces/Queen.h"
#include "pieces/Pawn.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Rook.h"

Move::Move(int x_from, int y_from, int x_to, int y_to, bool promotion, bool castling, bool en_passant)
        : x_from(x_from), y_from(y_from),
          x_to(x_to), y_to(y_to),
          promotion(promotion),
          castling(castling),
          en_passant(en_passant) {}

bool Move::match_dest_coords(int x, int y) const {
    return x_to == x and y_to == y;
}

void Move::execute(Board & board, std::list<std::shared_ptr<Piece>> & player_pieces) {

    // capture move
    if (board[x_to][y_to] != nullptr) {
        capture_piece = board[x_to][y_to];
        board[x_to][y_to]->capture();
    }

    // every single move
    board[x_from][y_from]->move(x_to, y_to, first_move);
    board[x_to][y_to] = board[x_from][y_from];
    board[x_from][y_from] = nullptr;

    // promotion
    if (promotion) {
        promoted = board[x_to][y_to];
        board[x_to][y_to]->change_availability(false);

        if (new_promotion_piece == nullptr)
            new_promotion_piece = std::make_shared<Queen>(x_to, y_to, player_pieces.front()->get_color());

        player_pieces.push_back(new_promotion_piece);
        board[x_to][y_to] = new_promotion_piece;
        return;
    }

    // castling short
    if (castling and y_to > y_from) {
        bool tmp;
        board[x_from][7]->move(x_to, 5, tmp);
        board[x_to][5] = board[x_from][7];
        board[x_from][7] = nullptr;
        return;
    }

    // castling long
    if (castling and y_to < y_from) {
        bool tmp;
        board[x_from][0]->move(x_to, 3, tmp);
        board[x_to][3] = board[x_from][0];
        board[x_from][0] = nullptr;
        return;
    }

    // en passant
    if (en_passant) {
        capture_piece = board[x_from][y_to];
        board[x_from][y_to]->capture();
        board[x_from][y_to] = nullptr;
    }
}

void Move::undo(Board & board, std::list<std::shared_ptr<Piece>> & player_pieces) {

    // en passant
    if (en_passant) {
        board[x_from][y_to] = capture_piece;
        capture_piece->un_capture(x_from, y_to);
        capture_piece = nullptr;
    }

    // castling short
    if (castling and y_to > y_from) {
        board[x_from][5]->undo_move(x_to, 7, true);
        board[x_to][7] = board[x_from][5];
        board[x_from][5] = nullptr;
    }

    // castling long
    if (castling and y_to < y_from) {
        board[x_from][3]->undo_move(x_to, 0, true);
        board[x_to][0] = board[x_from][3];
        board[x_from][3] = nullptr;
    }

    // promotion
    if (promotion) {
        player_pieces.remove(board[x_to][y_to]);
        promoted->change_availability(true, x_to, y_to);
        board[x_to][y_to] = promoted;
    }

    // every single move
    board[x_to][y_to]->undo_move(x_from, y_from, first_move);
    board[x_from][y_from] = board[x_to][y_to];
    board[x_to][y_to] = nullptr;

    // capture move
    if (capture_piece != nullptr) {
        board[x_to][y_to] = capture_piece;
        capture_piece->un_capture(x_to, y_to);
        capture_piece = nullptr;
    }
}

int Move::get_x_to() const {
    return x_to;
}

int Move::get_y_to() const {
    return y_to;
}

bool Move::match_coords(int x_from_p, int y_from_p, int x_to_p, int y_to_p) const {
    return x_from == x_from_p and y_from == y_from_p and x_to == x_to_p and y_to == y_to_p;
}

int Move::get_x_from() const {
    return x_from;
}

int Move::get_y_from() const {
    return y_from;
}

bool Move::is_promotion() const {
    return promotion;
}

void Move::set_new_promotion_piece(const std::string & piece_str, Color color) {
    if (piece_str == "queen") {
        new_promotion_piece = std::make_shared<Queen>(x_to, y_to, color);
        return;
    }

    if (piece_str == "knight") {
        new_promotion_piece = std::make_shared<Knight>(x_to, y_to, color);
        return;
    }

    if (piece_str == "bishop") {
        new_promotion_piece = std::make_shared<Bishop>(x_to, y_to, color);
        return;
    }

    if (piece_str == "rook") {
        new_promotion_piece = std::make_shared<Rook>(x_to, y_to, color);
        return;
    }
}

bool Move::is_castling() const {
    return castling;
}



