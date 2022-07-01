#include <limits>
#include <map>
#include <filesystem>
#include "Interface.h"
#include "Game.h"
#include "exceptions/UserInputException.h"
#include "exceptions/SaveFileException.h"

const char * const CLEAR = "\x1B[2J\x1B[H";

Interface::Interface(std::ostream & out, std::istream & in) : out(out), in(in) {}

std::string Interface::prompt_command(const std::function<bool(const std::string &)> & is_valid) const {
    while (true) {
        out << "Enter a command, type \"help\" to show instructions." << std::endl;

        std::string command;
        if (!(in >> command)) {
            clear();
            throw UserInputException("An unexpected error occurred during loading user input.");
        }

        transform(command.begin(), command.end(), command.begin(), ::tolower);

        if (not is_valid(command)) {
            out << "Invalid command!" << std::endl;
            clear();
            continue;
        }

        if (command == "move" or command == "save")
            return command;

        clear();
        return command;
    }
}

void Interface::clear() const {
    in.clear();
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Interface::print_string(const std::string & str) const {
    out << str << std::endl;
}

void Interface::print_help(const std::string & name, const std::string & description) const {
    out << "  - " << name << ": " << description << std::endl;
}

void Interface::clear_terminal() const {
    out << CLEAR << std::endl;
}

void Interface::print_game(Game & game, const std::string & print_move) const {
    clear_terminal();
    game.print(out, print_move);
}

bool Interface::prompt_coords(std::string & str) const {
    if (!(in >> str))
        throw UserInputException("An unexpected error occurred during loading input.");

    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    std::array<char, 8> chars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    std::array<char, 8> nums = {'1', '2', '3', '4', '5', '6', '7', '8'};

    if (str.length() != 4
        or std::find(chars.begin(), chars.end(), str[0]) == chars.end()
        or std::find(chars.begin(), chars.end(), str[2]) == chars.end()
        or std::find(nums.begin(), nums.end(), str[1]) == nums.end()
        or std::find(nums.begin(), nums.end(), str[3]) == nums.end()) {
        out << "Invalid coordinates! Correct format is \"move [from][to]\" (\"move a1a2\")" << std::endl;
        clear();
        return false;
    }

    clear();
    return true;
}

std::string Interface::prompt_file_name_load(const char * SAVES_DIR) const {
    if (not std::filesystem::exists(SAVES_DIR) or std::filesystem::is_empty(SAVES_DIR))
        throw SaveFileException("You have no saved games.");

    out << "Which game do you want to load?" << std::endl;
    std::filesystem::directory_iterator dir_iter(SAVES_DIR);
    for (const auto & file: dir_iter)
        out << " - " << file.path().filename() << std::endl;

    out << "Enter save file name: ";
    std::string file_name;
    if (!(in >> file_name))
        throw UserInputException("An unexpected error occurred during loading input.");
    clear();
    return file_name;
}

std::string Interface::prompt_file_name_save() const {
    std::string file_name;
    if (!(in >> file_name))
        throw UserInputException("An unexpected error occurred during loading input.");
    clear();
    return file_name;
}

std::string Interface::prompt_promotion() const {
    std::string piece_str;

    while (true) {
        out << "What piece do you want to promote? [queen, knight, rook, bishop]" << std::endl << std::flush;
        if (!(in >> piece_str))
            throw UserInputException("An unexpected error occurred during loading input.");

        std::transform(piece_str.begin(), piece_str.end(), piece_str.begin(), ::tolower);

        if (piece_str == "queen" or piece_str == "rook" or piece_str == "bishop" or piece_str == "knight")
            return piece_str;

        out << "Invalid promotion piece. Enter again." << std::endl;
    }
}

bool Interface::prompt_resign() const {
    std::string resign;
    out << "Do you really want to resign the game? [yes/no]" << std::endl;
    if (!(in >> resign))
        throw UserInputException("An unexpected error occurred during loading input.");

    std::transform(resign.begin(), resign.end(), resign.begin(), ::tolower);
    if (resign == "yes" or resign == "y" or resign == "yep" or resign == "yeah")
        return true;
    return false;
}
