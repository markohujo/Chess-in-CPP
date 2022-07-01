#include "Color.h"

const char * WHITE_STR = "WHITE";
const char * BLACK_STR = "BLACK";

std::ostream & operator<<(std::ostream & out, Color color) {
    color == Color::WHITE ? out << WHITE_STR : out << BLACK_STR;
    return out;
}

Color operator!(Color color) {
    Color result;
    color == Color::WHITE ? result = Color::BLACK : result = Color::WHITE;
    return result;
}

std::string to_string(Color color) {
    std::string str;
    color == Color::WHITE ? str = WHITE_STR : str = BLACK_STR;
    return str;
}