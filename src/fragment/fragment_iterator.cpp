#include "fragment/fragment.h"

namespace MemoryBucket {
    Fragment::Iterator& Fragment::Iterator::operator=(const Fragment::Iterator& other) {
        if (this == &other) {return *this;}
        *current = *other.current;
        *next = *other.next;
        *prev = *other.prev;
        return *this;
    }
    bool Fragment::Iterator::operator==(const Fragment::Iterator& other) const {
        return (current == other.current && prev == other.prev && next == other.next);
    }
    Fragment::Iterator& Fragment::Iterator::operator++() {
        current = next->current;
        prev = this;
        next = next->next;
        return *this;
    }
    Fragment::Iterator& Fragment::Iterator::operator--() {
        current = prev->current;
        next = this;
        prev = prev->prev;
        return *this;
    }
    const Fragment::Iterator& Fragment::Iterator::operator+(size_t n) const {
        auto r = Iterator(*this);
        for (size_t i = 0; i < n; ++i) {++r;}
        return r;
    }
    const Fragment::Iterator& Fragment::Iterator::operator-(size_t n) const {
        auto r = Iterator(*this);
        for (size_t i = 0; i < n; ++i) {--r;}
        return r;
    }
    size_t Fragment::Iterator::operator-(const Iterator& other) const{
        auto r = Iterator(*this);
        size_t s = 0;
        while (r!=other) {--r;++s;}
        return s;
    }
}