#include "fragment/fragment.h"

namespace MemoryBucket {
    Fragment::Fragment() : begin(nullptr), end(nullptr), size(0) {}
    Fragment::Fragment(Cell* data[], size_t size): size(size) {
        this->begin = new Iterator{data[0], nullptr, nullptr};
        Iterator *prev = this->begin, *current = nullptr;
        for (size_t i = 1; i<size; ++i) {
            current = new Iterator{data[i], nullptr, prev};
            prev->next = current;
        }
        this->end = current;
    }
    Fragment::Fragment(const Fragment& other) {
        size = other.size;
        this->begin = new Iterator{ &other.getFirst(), nullptr, nullptr};
        Iterator *prev = this->begin, *current = nullptr;
        for (size_t i = 1; i<size; ++i) {
            current = new Iterator{ &other[i], nullptr, prev};
            prev->next = current;
        }
        this->end = current;
    }
    Fragment::Fragment(Fragment&& other) noexcept {
        this->begin = other.begin;
        this->end = other.end;
        this->size = other.size;
        other.begin = nullptr;
        other.end = nullptr;
        other.size = 0;
    }
    Fragment::~Fragment() {
        Iterator *it = begin, *next = begin->next;
        while (it != end) {
            delete it;
            it = next;
            if (it) next = it->next;
        }
    }

    Fragment& Fragment::operator=(const Fragment& other) {
        if (this == &other) return *this;

        size_t min = (other.getSize() < size) ? other.getSize() : size;
        for (size_t i=0; i<min; ++i) {
            (begin+i)->current = &(other[i]);
        }
        for (size_t i=size-1; i>min-1; --i) {
            (begin+i)->current = nullptr;
        }
        return *this;
    }
    Fragment& Fragment::operator=(Fragment&& other) noexcept {
        this->begin = other.begin;
        this->end = other.end;
        this->size = other.size;
        other.begin = nullptr;
        other.end = nullptr;
        other.size = 0;
        return *this;
    }
    Cell& Fragment::operator[](size_t index) const { return *(begin+index)->current; }

    size_t Fragment::getSize() const { return size; }
    Cell& Fragment::getFirst() const { return *begin->current;}
    Cell& Fragment::getLast() const { return *end->current;}
    Fragment::Iterator Fragment::getIterator() const {return *begin;}
}