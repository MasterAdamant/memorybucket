#include "cell/cell.h"

#include <iostream>

using namespace MemoryBucket;
using namespace std;

void expect_test(int current, int expected) {
    cout << "Result: " << current << " (expected: " << expected << ") => " << ((current == expected) ? "Success!" : "Failure!") << "\n";
}

int main() {
    //Status: Cell size is not initialised
    auto creation_result = Cell::create(nullptr, {});
    expect_test(static_cast<uint16_t>(creation_result.first), 1);

    //Status: Zero initialization attempt
    auto init_result = Cell::initialize(0);
    expect_test(static_cast<uint16_t>(init_result), 2);

    //Status: Successful initialization
    init_result = Cell::initialize(4);
    expect_test(static_cast<uint16_t>(init_result), 0);

    //Status: Repeated initialization
    init_result = Cell::initialize(8);
    expect_test(static_cast<uint16_t>(init_result), 1);

    //Status: Null pointer
    creation_result = Cell::create(nullptr, {});
    expect_test(static_cast<uint16_t>(creation_result.first), 2);

    //Status: Success creation of Cell
    byte* ptr = new byte[4];
    byte data[] {byte(0b0),byte(0b0),byte(0b0),byte(0b0)};
    creation_result = Cell::create(ptr, data);
    expect_test(static_cast<uint16_t>(creation_result.first), 0);

    delete[] ptr;
    return 0;
}