#include "UserInputException.h"

UserInputException::UserInputException(const char * string) : runtime_error(string) {}

const char * UserInputException::what() const noexcept {
    return runtime_error::what();
}
