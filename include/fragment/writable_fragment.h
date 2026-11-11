#ifndef MEMORYBUCKET_WRITABLE_FRAGMENT_H
#define MEMORYBUCKET_WRITABLE_FRAGMENT_H

#include "fragment.h"
namespace MemoryBucket{
    class WritableFragment: public Fragment {
        WritableFragment();
        WritableFragment(Cell* begin, Cell* end);
        WritableFragment(const WritableFragment&);
        WritableFragment(WritableFragment&&) noexcept;
        ~WritableFragment() override;

        WritableFragment& operator=(const WritableFragment&);
        WritableFragment& operator=(WritableFragment&&) noexcept;
        Cell& operator[](size_t index);

        void set(size_t cellIndex, size_t bit_index, std::byte value);
        void set(size_t total_index, std::byte value);
    };
}


#endif //MEMORYBUCKET_WRITABLE_FRAGMENT_H