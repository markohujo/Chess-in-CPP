#pragma once

#include <map>
#include "Command.h"

/**
 * Static class for creating Command objects when application starts (in Application constructor)
 */
class CommandFactory {
public:
    CommandFactory() = delete;

    static Command help(const std::map<std::string, Command> & commands);

    static Command new_game(std::map<std::string, Command> & commands);

    static Command single(std::map<std::string, Command> & commands);

    static Command easy(std::map<std::string, Command> & commands);

    static Command medium(std::map<std::string, Command> & commands);

    static Command hard(std::map<std::string, Command> & commands);

    static Command versus(std::map<std::string, Command> & commands);

    static Command move();

    static Command resign();

    static Command board();

    static Command captured();

    static Command save();

    static Command load(std::map<std::string, Command> & commands);

    static Command quit();

private:
    static void update_commands_level(std::map<std::string, Command> & commands);
};
