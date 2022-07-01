#pragma once

#include <stdexcept>

/**
 * This exception is thrown if an error occurs when saving or loading from or to a save file
 */
class SaveFileException : public std::runtime_error {
public:
    explicit SaveFileException(const char * string);

    const char * what() const noexcept override;
};
