#pragma once

#include <map>
#include <string>
#include "Interface.h"
#include "Game.h"
#include "Command.h"

/**
 * Represents one running application, contains Interface object for IO operations and Game object for controlling the actual game
 */
class Application {
public:
    explicit Application(const Interface & interface);

    /** Copy constructor is deleted because application cannot be copied */
    Application(const Application & src) = delete;

    /** Operator= is deleted because application cannot be copied (assigned) */
    Application & operator=(const Application & src) = delete;

    ~Application() = default;

    /**
     * @throw LoadingInputException if an unexpected error occurs during loading user input (stops the application).
     * Catches SaveFileException errors as this error does not stop the application
     * @return EXIT_SUCCESS (0) if app stops running successfully
     */
    int run();

private:
    Interface interface;
    Game game;
    std::map<std::string, Command> commands;
    bool running = true;
};
