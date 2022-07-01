#include "Command.h"

#include <utility>

Command::Command(std::string format, std::string description, std::function<bool(Interface &, Game &)> executeFunction,
                 std::list<GameState> availableStates, bool available)
        : format(std::move(format)),
          description(std::move(description)),
          execute_function(std::move(executeFunction)),
          available_states(std::move(availableStates)),
          available(available) {}

bool Command::is_available() const {
    return available;
}

bool Command::execute(Interface & interface, Game & game) {
    return execute_function(interface, game);
}

const std::string & Command::get_description() const {
    return description;
}

bool Command::available_in(GameState state) const {
    return std::find(available_states.begin(), available_states.end(), state) != available_states.end();
}

void Command::change_availability(bool available_param) {
    available = available_param;
}

const std::string & Command::get_format() const {
    return format;
}
