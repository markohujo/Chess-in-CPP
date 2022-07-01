#include "Application.h"
#include "exceptions/UserInputException.h"

int main() {
    try {
        return Application(Interface(std::cout, std::cin)).run();
    } catch (const UserInputException & e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
