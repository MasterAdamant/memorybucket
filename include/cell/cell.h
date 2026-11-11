#ifndef MEMORYBUCKET_CELL_H
#define MEMORYBUCKET_CELL_H

#include <utility>

#include "cell_status.h"

//Use size_t without additional imports
using size_t = decltype(sizeof(0));

namespace MemoryBucket {
    //CELL
    //Represents chunk of memory with fixed size.
    //Needs to be setup up before using with calling a method with specifying size of cell
    class Cell {
        public:
        //CONSTRUCTORS
        //Can be used to either create null cells (for loops) or moving data from one to another
        //Should not be used for cell initialization, see: Cell::create(...)
        Cell();
        Cell(const Cell& cell) = delete;
        Cell(Cell&& cell) noexcept;
        ~Cell();

        Cell& operator= (const Cell& cell);
        Cell& operator= (Cell&& cell) noexcept;

        //Direct access to the stored bytes
        std::byte& operator[] (size_t index) const;

        static size_t size() noexcept;
        //Used for setting up cells size. Makes nothing on second and later calls
        static CellInitializationStatus initialize(size_t cellSize);
        //Creates new cell with possible few outcomes except success, such as size is still not initialised and etc.
        //Wraps memory into a Cell and writes data to it
        static std::pair<CellCreationStatus, Cell> create (void* ptr, std::byte data[]);

        //SYNTAX SUGAR
        //Check whether cell has any data
        bool empty() const noexcept;


        private:
        //Private constructor wrapping memory bytes
        Cell(std::byte* ptr);

        static size_t cellSize;         //Size in bytes, can be get by sizeof(Type)
        std::byte* data;                //Data of cell

    };
}

#endif //MEMORYBUCKET_CELL_H