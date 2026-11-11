#include "fragment/fragment.h"

#include <iostream>

using namespace MemoryBucket;
using namespace std;

void expect_test(int current, int expected) {
    cout << "Result: " << current << " (expected: " << expected << ") => " << ((current == expected) ? "Success!" : "Failure!") << "\n";
}

// Helper: wraps `storage` (4 bytes, leaked like the rest of this test's
// scratch memory) in a Cell whose bytes are all set to `v`.
Cell makeCell(std::byte v) {
    std::byte* storage = new std::byte[Cell::size()];
    std::byte data[4] {v, v, v, v};
    auto result = Cell::create(storage, data);
    return std::move(result.second);
}

int main() {
    Cell::initialize(4);

    // ---- Fixtures ----
    // Two independent cells for the "normal" size-2 test zone. Note:
    // operator[]/copy-ctor/copy-assignment are only reliable at index 0
    // (see bug notes below), so size 2 is the largest fragment size where
    // *every* public accessor still behaves correctly.
    Cell a = makeCell(std::byte(1));
    Cell b = makeCell(std::byte(2));
    Cell* pair2[2] {&a, &b};

    //=====================================================================
    // Default construction
    //=====================================================================

    //Status: Default-constructed fragment has size 0
    // NOTE: intentionally heap-allocated and never deleted. Fragment's
    // destructor unconditionally dereferences `begin->next`; for a
    // default-constructed fragment `begin` is null, so destroying it
    // segfaults (confirmed). Leaking is the only way to exercise this
    // constructor without crashing the whole test binary.
    Fragment* emptyFrag = new Fragment();
    expect_test(static_cast<int>(emptyFrag->getSize()), 0);

    //=====================================================================
    // Construction from an array of Cell*
    //=====================================================================

    //Status: Fragment built from 2 cells reports the right size
    Fragment frag2(pair2, 2);
    expect_test(static_cast<int>(frag2.getSize()), 2);

    //Status: getFirst() returns the first cell
    expect_test(&frag2.getFirst() == &a, true);

    //Status: getLast() returns the last cell
    expect_test(&frag2.getLast() == &b, true);

    //Status: operator[](0) returns the first cell
    expect_test(&frag2[0] == &a, true);

    // NOTE: operator[](index) for index > 0 is not tested. It computes
    // `begin + index` as raw pointer arithmetic on `Iterator*`, but each
    // Iterator node is an independently heap-allocated linked-list node,
    // not a contiguous array element. Confirmed by direct testing: index 1
    // and 2 silently return garbage Cell references, and index 3 segfaults
    // reading unmapped memory. The result is undefined behavior that can
    // crash nondeterministically, so it is deliberately not exercised here.

    //=====================================================================
    // Iterator basics
    //=====================================================================

    //Status: getIterator() starts at the first cell
    Fragment::Iterator it = frag2.getIterator();
    expect_test(it.current == &a, true);

    //Status: two independently-obtained iterators to the same fragment compare equal
    Fragment::Iterator itOther = frag2.getIterator();
    expect_test(it == itOther, true);

    //Status: incrementing the iterator moves to the second cell (size-2 fragment)
    ++it;
    expect_test(it.current == &b, true);

    // NOTE: Iterator::operator+(n) / operator-(n) are not tested. Both
    // return `const Iterator&` bound to a local variable that is destroyed
    // when the function returns (confirmed by compiler warning
    // -Wreturn-local-addr and by inspection): the reference is dangling
    // the instant the caller receives it. Any use of the result is
    // undefined behavior.

    // NOTE: Iterator::operator-(const Iterator&) (distance) is not tested.
    // It repeatedly applies operator-- until the two iterators compare
    // equal. operator++/operator-- set `prev`/`next` to the address of
    // the *current* Iterator object (`this`) rather than preserving the
    // original list-node links, so once an iterator has been
    // incremented/decremented it can become self-referential. Confirmed
    // by direct testing: computing a distance between such iterators
    // hangs in an infinite loop, so it is deliberately not exercised here.

    // NOTE: Iterator::operator=(const Iterator&) is not tested. Rather
    // than repointing `this`, it dereferences and mutates *current,
    // *next and *prev on both sides. For boundary nodes (e.g. the first
    // iterator, whose `prev` is null) this dereferences a null pointer
    // and crashes, so it is deliberately not exercised here.

    //=====================================================================
    // Copy semantics
    //=====================================================================

    //Status: copy constructor preserves size
    Fragment copy2(frag2);
    expect_test(static_cast<int>(copy2.getSize()), 2);

    //Status: copy constructor's first element matches the original
    expect_test(&copy2.getFirst() == &a, true);

    //Status: copy constructor's last element matches the original (BUG)
    // The copy constructor fetches every element after the first via
    // `other[i]`, i.e. Fragment::operator[], which (as noted above) is
    // broken for index > 0. Confirmed: the copy's last element does not
    // actually point at `b`, so this reports Failure rather than Success.
    expect_test(&copy2.getLast() == &b, true);

    //Status: copy assignment copies the first element between equal-sized fragments
    Cell c = makeCell(std::byte(3));
    Cell d = makeCell(std::byte(4));
    Cell* pair2b[2] {&c, &d};
    Fragment assigned(pair2b, 2);
    assigned = frag2;
    expect_test(&assigned.getFirst() == &a, true);

    //=====================================================================
    // Move semantics
    //=====================================================================

    //Status: move constructor transfers size to the destination
    // NOTE: `moveSrc` is heap-allocated and deliberately never deleted,
    // for the same reason `emptyFrag` above is leaked: after being moved
    // from, its `begin` becomes null, and destroying it would segfault in
    // Fragment::~Fragment (confirmed).
    Fragment* moveSrc = new Fragment(pair2, 2);
    Fragment moved(std::move(*moveSrc));
    expect_test(static_cast<int>(moved.getSize()), 2);

    //Status: move constructor leaves the source with size 0
    expect_test(static_cast<int>(moveSrc->getSize()), 0);

    //Status: moved-to fragment's first element matches the original
    expect_test(&moved.getFirst() == &a, true);

    //Status: move assignment transfers size to the destination
    Fragment* moveSrc2 = new Fragment(pair2, 2);
    Fragment moveTarget(pair2b, 2);
    moveTarget = std::move(*moveSrc2);
    expect_test(static_cast<int>(moveTarget.getSize()), 2);

    //Status: move assignment leaves the source with size 0
    expect_test(static_cast<int>(moveSrc2->getSize()), 0);

    //=====================================================================
    // Single-element fragment (edge case)
    //=====================================================================

    Cell e = makeCell(std::byte(5));
    Cell* single[1] {&e};
    Fragment frag1(single, 1);

    //Status: single-element fragment reports size 1
    expect_test(static_cast<int>(frag1.getSize()), 1);

    //Status: single-element fragment's first element is correct
    expect_test(&frag1.getFirst() == &e, true);

    // NOTE: frag1.getLast() is not tested. The array constructor only
    // assigns `this->end` inside its loop, but for size == 1 the loop body
    // never runs, so `end` is left null. getLast() dereferences
    // `end->current`, which segfaults on a null `end` (confirmed by direct
    // testing), so it is deliberately not exercised here.

    //=====================================================================
    // Multi-element fragment, size >= 3 (reveals a linked-list bug)
    //=====================================================================

    Cell f = makeCell(std::byte(6));
    Cell g = makeCell(std::byte(7));
    Cell h = makeCell(std::byte(8));
    Cell* triple[3] {&f, &g, &h};
    Fragment frag3(triple, 3);

    //Status: 3-element fragment reports size 3
    expect_test(static_cast<int>(frag3.getSize()), 3);

    //Status: 3-element fragment's first element is correct
    expect_test(&frag3.getFirst() == &f, true);

    //Status: 3-element fragment's last element is correct
    // (getLast() reads `end` directly, which the constructor does set
    // correctly, so this one is unaffected by the bug below.)
    expect_test(&frag3.getLast() == &h, true);

    //Status: incrementing from the first element reaches the second element (BUG)
    // The array constructor's loop never advances its `prev` pointer past
    // `begin` before linking the next node in (it always links new nodes
    // as `begin`'s neighbour), so for fragments of 3+ elements the only
    // reachable link from `begin` is the *last* node, not the second one.
    // Confirmed by direct testing: incrementing past the first element
    // jumps straight to the last cell, skipping every cell in between, so
    // this reports Failure rather than Success.
    Fragment::Iterator it3 = frag3.getIterator();
    ++it3;
    expect_test(it3.current == &g, true);

    //=====================================================================
    // Templates that are not exercised
    //=====================================================================
    // Fragment::readAs<Type> is not tested: its body declares `Type
    // result;` but never returns it and never touches its `frag`
    // parameter, so any instantiation reaches the end of a non-void
    // function without a return value (undefined behavior) and does not
    // actually decode anything.
    //
    // Fragment::getFrom<Type> is not tested: it is declared but has no
    // definition anywhere in the library, so instantiating/calling it
    // would fail to link.

    cout << "\nAll Fragment tests completed.\n";
    return 0;
}