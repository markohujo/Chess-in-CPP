#include "SaveFileException.h"

SaveFileException::SaveFileException(const char * string) : runtime_error(string) {}

const char * SaveFileException::what() const noexcept {
    return runtime_error::what();
}
