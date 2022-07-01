#include "Piece.h"

Piece::Piece(int x, int y, Color color) : x(x), y(y), color(color) {

}

std::ostream & operator<<(std::ostream & out, const Piece & piece) {
    return piece.print(out, false);
}

bool Piece::match_coords(int x_p, int y_p) const {
    return x == x_p and y == y_p;
}

void Piece::capture() {
    x = -1;
    y = -1;
    available = false;
    captured = true;
}

void Piece::un_capture(int x_p, int y_p) {
    available = true;
    captured = false;
    x = x_p;
    y = y_p;
}

bool Piece::match_color(Color color_p) const {
    return color == color_p;
}

bool Piece::is_available() const {
    return available;
}

Color Piece::get_color() const {
    return color;
}

void Piece::change_availability(bool a, int x_p, int y_p) {
    available = a;
    x = x_p;
    y = y_p;
}

bool Piece::has_already_moved() const {
    return has_moved;
}

bool Piece::was_captured() const {
    return captured;
}

int Piece::get_x() const {
    return x;
}

int Piece::get_y() const {
    return y;
}

void Piece::set_has_moved(bool moved) {
    has_moved = moved;
}

void Piece::move(int x_new, int y_new, bool & first_move) {
    x = x_new;
    y = y_new;

    // if this is this piece's first move
    if (not has_moved) {
        first_move = true;
        has_moved = true;
    }
}

void Piece::undo_move(int x_new, int y_new, bool undo_has_moved) {
    x = x_new;
    y = y_new;
    if (undo_has_moved)
        has_moved = false;
}
