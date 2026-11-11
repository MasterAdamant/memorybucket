#ifndef MEMORYBUCKET_FRAGMENT_H
#define MEMORYBUCKET_FRAGMENT_H

#include "cell/cell.h"

namespace MemoryBucket {
    class Fragment {
        public:
            Fragment();
            Fragment(Cell* data[], size_t size);
            Fragment(const Fragment&);
            Fragment(Fragment&&) noexcept;
            virtual ~Fragment();

            Fragment& operator=(const Fragment&);
            Fragment& operator=(Fragment&&) noexcept;
            Cell& operator[](size_t index) const;

            size_t getSize() const;
            Cell& getFirst() const;
            Cell& getLast() const;

            template<typename Type>
            static Type readAs(Fragment frag) {
                Type result;
                
            }
            template<typename Type>
            static Fragment getFrom(Type value);

            struct Iterator {
                Cell* current;
                Iterator *next, *prev;
                Iterator& operator=(const Iterator& other);
                bool operator==(const Iterator& other) const;
                Iterator& operator++();
                Iterator& operator--();
                const Iterator& operator+(size_t n) const;
                const Iterator& operator-(size_t n) const;
                size_t operator-(const Iterator& other) const;
            };

            Iterator getIterator() const;
        private:
            Iterator *begin, *end;
            size_t size;
    };
}



#endif //MEMORYBUCKET_FRAGMENT_H
