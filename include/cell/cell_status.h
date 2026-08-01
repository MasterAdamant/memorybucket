#ifndef CELL_ERRORS_H
#define CELL_ERRORS_H

#include <cstdint>

namespace MemoryBucket {
    enum struct CellInitializationStatus : std::uint16_t {
        SUCCESS = 0,
        ALREADY_INITIALIZED = 1,
        MUST_BE_NON_ZERO = 2,
    };
    enum struct CellCreationStatus : std::uint16_t {
        SUCCESS = 0,
        UNINITIALIZED_SIZE = 1,
        NULL_POINTER = 2,
    };
}

#endif //CELL_ERRORS_H