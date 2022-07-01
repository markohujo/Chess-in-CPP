#pragma once


#include <iostream>
#include <functional>
#include "Game.h"

class Game;

/**
 * UI class, used for user input and output.
 * Prompts user for commands, files, etc.
 * Informs user about game state, shows help, etc.
 */
class Interface {
public:
    Interface(std::ostream & out, std::istream & in);

    /**
     * @throw UserInputException if an unexpected error occurs during loading user input
     */
    std::string prompt_command(const std::function<bool(const std::string &)> & is_valid) const;

    /**
     * @throw UserInputException if an unexpected error occurs during loading user input
     */
    bool prompt_coords(std::string & str) const;

    /**
     * @throw UserInputException if an unexpected error occurs during loading user input
     */
    std::string prompt_file_name_load(const char * SAVES_DIR) const;

    /**
     * @throw UserInputException if an unexpected error occurs during loading user input
     */
    std::string prompt_file_name_save() const;

    /**
     * @throw UserInputException if an unexpected error occurs during loading user input
     */
    std::string prompt_promotion() const;

    /**
     * @throw UserInputException if an unexpected error occurs during loading user input
     */
    bool prompt_resign() const;

    void print_string(const std::string & str) const;

    void print_help(const std::string & name, const std::string & description) const;

    void print_game(Game & game, const std::string & print_move) const;

    void clear_terminal() const;

private:
    std::ostream & out;

    std::istream & in;

    void clear() const;
};
