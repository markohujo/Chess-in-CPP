#pragma once

#include <stdexcept>

/**
 * This exception is thrown if an error occurs when loading input from user
 */
class UserInputException : public std::runtime_error {
public:
    explicit UserInputException(const char * string);

    const char * what() const noexcept override;
};
