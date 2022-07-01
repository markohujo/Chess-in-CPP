#include "Application.h"
#include <utility>
#include "CommandFactory.h"
#include "exceptions/SaveFileException.h"

Application::Application(const Interface & interface) : interface(interface) {
    commands.emplace("help", CommandFactory::help(commands));
    commands.emplace("new", CommandFactory::new_game(commands));
    commands.emplace("single", CommandFactory::single(commands));
    commands.emplace("easy", CommandFactory::easy(commands));
    commands.emplace("medium", CommandFactory::medium(commands));
    commands.emplace("hard", CommandFactory::hard(commands));
    commands.emplace("versus", CommandFactory::versus(commands));
    commands.emplace("move", CommandFactory::move());
    commands.emplace("resign", CommandFactory::resign());
    commands.emplace("board", CommandFactory::board());
    commands.emplace("captured", CommandFactory::captured());
    commands.emplace("save", CommandFactory::save());
    commands.emplace("load", CommandFactory::load(commands));
    commands.emplace("quit", CommandFactory::quit());
}

int Application::run() {
    interface.clear_terminal();
    while (running) {
        try {
            auto command = interface.prompt_command([this](const std::string & command_str) {
                return commands.find(command_str) != commands.end() && commands[command_str].is_available();
            });
            running = commands[command].execute(interface, game);
        } catch (const SaveFileException & e) {
            std::cerr << e.what() << std::endl;
            running = true;
        }
    }
    return EXIT_SUCCESS;
}
