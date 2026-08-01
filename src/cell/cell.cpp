#include "../../include/cell/cell.h"

#include <cstring>

namespace MemoryBucket {
    //Constructors
    Cell::Cell(): data(nullptr) {}
    Cell::Cell(std::byte* ptr): data(ptr) {}
    Cell::~Cell() = default;

    Cell::Cell(Cell&& cell) noexcept : data(nullptr){
        *this = std::move(cell);
    }

    Cell& Cell::operator= (const Cell& cell) {
        if (this == &cell) return *this;
        memcpy(data, cell.data, sizeof(std::byte) * size());
        return *this;
    }
    Cell& Cell::operator= (Cell&& cell) noexcept{
        if (this == &cell) return *this;
        data = cell.data;
        return *this;
    }
    std::byte& Cell::operator[] (size_t index) const {
        return data[index];
    }

    CellInitializationStatus Cell::initialize(size_t cellSize) {
        if (size() != 0)
            return CellInitializationStatus::ALREADY_INITIALIZED;
        if (cellSize == 0)
            return CellInitializationStatus::MUST_BE_NON_ZERO;
        Cell::cellSize = cellSize;
        return CellInitializationStatus::SUCCESS;
    }
    size_t Cell::size() noexcept {return cellSize;}
    std::pair<CellCreationStatus, Cell> Cell::create(void* ptr, std::byte data[]) {
        auto result = CellCreationStatus::SUCCESS;

        if (size() == 0)  result = CellCreationStatus::UNINITIALIZED_SIZE;
        else if (ptr == nullptr) result = CellCreationStatus::NULL_POINTER;
        else if (result == CellCreationStatus::SUCCESS) {
            memcpy(ptr, data, sizeof(std::byte) * size());
        }

        return {result, Cell(static_cast<std::byte*>(ptr))};
    }

    bool Cell::empty() const noexcept { return !data;}


    size_t Cell::cellSize = 0;
}
