#pragma once

#include <string>
#include <functional>
#include <list>
#include "Interface.h"
#include "Game.h"
#include "enums/GameState.h"

/**
 * Represents a command that user can enter and execute
 */
class Command {
public:
    Command() = default;

    Command(std::string format, std::string description, std::function<bool(Interface &, Game &)> executeFunction,
            std::list<GameState> availableStates, bool available);

    /**
     * Executes this command's execute function
     * @return true if game still continues
     * @return false if game ends after this command is executed
     */
    bool execute(Interface & interface, Game & game);

    /**
     * Checks if this command is available in the given state
     * @return true if this command is available in the given state, false ohterwise
     */
    bool available_in(GameState state) const;

    /**
     * Changes this command's availability to the given parameter (setter for available)
     * @param available_param value to which this command's availability will change
     */
    void change_availability(bool available_param);

    /**
     * @return true if this command is currently available (getter for available field)
     */
    bool is_available() const;

    const std::string & get_format() const;

    const std::string & get_description() const;

private:
    std::string format;

    std::string description;

    /** Function that gets executed when user enters this command */
    std::function<bool(Interface &, Game &)> execute_function;

    /** List of game states in which this command is available */
    std::list<GameState> available_states;

    bool available = false;
};
