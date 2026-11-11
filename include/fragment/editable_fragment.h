#ifndef MEMORYBUCKET_EDITABLE_FRAGMENT_H
#define MEMORYBUCKET_EDITABLE_FRAGMENT_H

#include "writable_fragment.h"

namespace MemoryBucket {
    class EditableFragment : public WritableFragment {
        EditableFragment();
        EditableFragment(Cell* begin, Cell* end);
        EditableFragment(const EditableFragment&);
        EditableFragment(EditableFragment&&) noexcept;
        ~EditableFragment() override;

        EditableFragment& operator=(const EditableFragment&);
        EditableFragment& operator=(EditableFragment&&) noexcept;

        void insertCell(Cell* cell, size_t pos);
        void pushToBegin(Cell* cell);
        void pushToEnd(Cell* cell);
        bool removeCell(Cell* cell);
    };
}

#endif //MEMORYBUCKET_EDITABLE_FRAGMENT_H
