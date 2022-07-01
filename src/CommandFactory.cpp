#include <fstream>
#include <filesystem>
#include <thread>
#include "CommandFactory.h"
#include "exceptions/SaveFileException.h"

const char * SAVES_DIR = "saves/";

const char * RULES = "White pieces are marked with capital letters.\nP/p = pawn\nR/r = rook\nN/n = knight\nB/b = bishop\nQ/q = queen\nK/k = king";

Command CommandFactory::help(const std::map<std::string, Command> & commands) {
    return {"help", "Shows instructions.", [&commands](Interface & interface, Game &) {
        interface.clear_terminal();

#ifdef NOTPRETTY
        interface.print_string(RULES);
#endif

        interface.print_string("\nAvailable commands:");
        for (const auto & [name, command]: commands)
            if (command.is_available())
                interface.print_help(command.get_format(), command.get_description());
        return true;
    }, std::list<GameState>{GameState::MENU, GameState::GAME_MODE,
                            GameState::LEVEL, GameState::IN_GAME}, true};
}

Command CommandFactory::new_game(std::map<std::string, Command> & commands) {
    return {"new", "Creates new game.", [&commands](Interface & interface, Game &) {
        interface.clear_terminal();
        interface.print_string("Do you want to play single or versus mode? [single, versus]");

        for (auto & [name, command]: commands) {
            if (command.available_in(GameState::MENU) and
                not command.available_in(GameState::GAME_MODE))
                command.change_availability(false);
            else if (command.available_in(GameState::GAME_MODE))
                command.change_availability(true);
        }

        return true;
    }, std::list<GameState>{GameState::MENU}, true};
}

Command CommandFactory::single(std::map<std::string, Command> & commands) {
    return {"single", "Starts a new single player game", [&commands](Interface & interface, Game &) {
        interface.clear_terminal();
        interface.print_string("Choose level of your opponent. [easy, medium, hard]");

        for (auto & [name, command]: commands)
            if (command.available_in(GameState::GAME_MODE) and
                not command.available_in(GameState::LEVEL))
                command.change_availability(false);
            else if (command.available_in(GameState::LEVEL))
                command.change_availability(true);

        return true;
    }, std::list<GameState>{GameState::GAME_MODE}, false};
}

Command CommandFactory::versus(std::map<std::string, Command> & commands) {
    return {"versus", "Starts a new versus player game", [&commands](Interface & interface, Game & game) {
        interface.clear_terminal();
        game.initialize(GameMode::VERSUS);
        interface.print_game(game, std::string());

        for (auto & [name, command]: commands)
            if (command.available_in(GameState::GAME_MODE) and
                not command.available_in(GameState::IN_GAME))
                command.change_availability(false);
            else if (command.available_in(GameState::IN_GAME))
                command.change_availability(true);

        return true;
    }, std::list<GameState>{GameState::GAME_MODE}, false};
}

Command CommandFactory::quit() {
    return {"quit", "Ends the game without saving.", [](Interface & interface, Game &) {
        interface.clear_terminal();
        interface.print_string("Thanks for playing");
        return false;
    }, std::list<GameState>{GameState::MENU, GameState::GAME_MODE,
                            GameState::LEVEL, GameState::IN_GAME}, true};
}

Command CommandFactory::board() {
    return {"board", "Shows the current playing board and side on turn.", [](Interface & interface, Game & game) {
        interface.clear_terminal();
        interface.print_game(game, "");
        return true;
    }, std::list<GameState>{GameState::IN_GAME}, false};
}

Command CommandFactory::easy(std::map<std::string, Command> & commands) {
    return {"easy", "Creates a new single game with easy AI opponent.",
            [&commands](Interface & interface, Game & game) {
                game.initialize(GameMode::EASY);
                interface.print_game(game, "");
                update_commands_level(commands);
                return true;
            }, std::list<GameState>{GameState::LEVEL}, false};
}

Command CommandFactory::medium(std::map<std::string, Command> & commands) {
    return {"medium", "Creates a new single game with medium AI opponent.",
            [&commands](Interface & interface, Game & game) {
                game.initialize(GameMode::MEDIUM);
                interface.print_game(game, "");
                update_commands_level(commands);
                return true;
            }, std::list<GameState>{GameState::LEVEL}, false};
}

Command CommandFactory::hard(std::map<std::string, Command> & commands) {
    return {"hard", "Creates a new single game with hard AI opponent.",
            [&commands](Interface & interface, Game & game) {
                game.initialize(GameMode::HARD);
                interface.print_game(game, "");
                update_commands_level(commands);
                return true;
            }, std::list<GameState>{GameState::LEVEL}, false};
}

void CommandFactory::update_commands_level(std::map<std::string, Command> & commands) {
    for (auto & [name, command]: commands) {
        if (command.available_in(GameState::LEVEL) and
            not command.available_in(GameState::IN_GAME))
            command.change_availability(false);
        else if (command.available_in(GameState::IN_GAME))
            command.change_availability(true);
    }
}

Command CommandFactory::move() {
    return {"move <from><to>", "Moves piece. (\"move a1a2\")",
            [](Interface & interface, Game & game) {
                return game.move_player(interface);
            }, std::list<GameState>{GameState::IN_GAME}, false};
}

Command CommandFactory::save() {
    return {"save <filename>", "Saves the current game.", [](Interface & interface, Game & game) {
        std::string file_name = interface.prompt_file_name_save();

        if (file_name.find('.') != std::string::npos or file_name.find('~') != std::string::npos or
            file_name.find('/') != std::string::npos)
            throw SaveFileException("Invalid file name. Cannot contain \".\", \"~\" and \"/\"");

        std::filesystem::create_directory(SAVES_DIR);

        if (std::filesystem::exists(SAVES_DIR + file_name))
            throw SaveFileException("Save with this name already exists.");

        std::ofstream file(SAVES_DIR + file_name, std::ios::out);
        if (!file or not file.is_open())
            throw SaveFileException("Cannot open file.");
        if (!(file << game))
            throw SaveFileException("Saving failed.");
        interface.print_string("Game saved successfully as " + file_name);
        file.close();
        return true;
    }, std::list<GameState>{GameState::IN_GAME}, false};
}

Command CommandFactory::load(std::map<std::string, Command> & commands) {
    return {"load", "Loads previously saved game.", [&commands](Interface & interface, Game & game) {
        interface.clear_terminal();
        std::string file_name = interface.prompt_file_name_load(SAVES_DIR);

        if (file_name.find('.') != std::string::npos or file_name.find('~') != std::string::npos or
            file_name.find('/') != std::string::npos)
            throw SaveFileException("Invalid file name. Cannot contain \".\", \"~\" and \"/\"");

        std::ifstream file(SAVES_DIR + file_name, std::ios::in);
        if (!file or not file.is_open())
            throw SaveFileException("Cannot open file.");
        if (!(file >> game)) {
            std::filesystem::remove(SAVES_DIR + file_name);
            throw SaveFileException("Save file corrupted. I'm deleting this save.");
        }
        interface.print_game(game, std::string());
        file.close();

        for (auto & [name, command]: commands) {
            if (command.available_in(GameState::MENU) and
                not command.available_in(GameState::IN_GAME))
                command.change_availability(false);
            else if (command.available_in(GameState::IN_GAME))
                command.change_availability(true);
        }

        return true;
    }, std::list<GameState>{GameState::MENU}, true};
}

Command CommandFactory::captured() {
    return {"captured", "Shows all captured pieces.", [](Interface & interface, Game & game) {
        auto captured_pair = game.captured();
        interface.clear_terminal();

        std::string print = "White captured pieces:\n  ";
        if (captured_pair.first.empty())
            print = "No white captured pieces.";
        for (const auto & piece: captured_pair.first)
            print.append(to_string(piece->type())).append(" ");
        interface.print_string(print);

        print = "Black captured pieces:\n  ";
        if (captured_pair.second.empty())
            print = "No black captured pieces.";
        for (const auto & piece: captured_pair.second)
            print.append(to_string(piece->type())).append(" ");
        interface.print_string(print);

        return true;
    }, std::list<GameState>{GameState::IN_GAME}, false};
}

Command CommandFactory::resign() {
    return {"resign", "Resign the game and concede that your opponent has won without checkmate.",
            [](const Interface & interface, Game & game) {
                bool resign = interface.prompt_resign();
                if (not resign) {
                    interface.print_game(game, "Game not resigned.");
                    return true;
                }
                game.resign(interface);
                return false;
            }, std::list<GameState>{GameState::IN_GAME}, false};
}


