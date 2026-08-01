#ifndef MEMORYBUCKET_CELL_H
#define MEMORYBUCKET_CELL_H

#include <utility>

#include "cell_status.h"

using size_t = decltype(sizeof(0));

namespace MemoryBucket {
    class Cell {
        public:
        Cell();
        Cell(const Cell& cell) = delete;
        Cell(Cell&& cell) noexcept;
        ~Cell();

        Cell& operator= (const Cell& cell);
        Cell& operator= (Cell&& cell) noexcept;
        std::byte& operator[] (size_t index) const;

        static CellInitializationStatus initialize(size_t cellSize);
        static size_t size() noexcept;
        static std::pair<CellCreationStatus, Cell> create (void* ptr, std::byte data[]);

        bool empty() const noexcept;


        private:
        Cell(std::byte* ptr);

        static size_t cellSize;         //Size in bytes, can be get by sizeof(Type)
        std::byte* data;                //Data of cell

    };
}

#endif //MEMORYBUCKET_CELL_H