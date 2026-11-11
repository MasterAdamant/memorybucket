cmake_minimum_required(VERSION 3.10)

project(MemoryBucket VERSION 1.0)

set(CXX_STANDARD 11)
set(SOURCES
        tests/fragment_general.cpp
        src/cell/cell.cpp
        include/fragment/fragment.h
        src/fragment/fragment.cpp
        include/fragment/editable_fragment.h
        src/fragment/editable_fragment.cpp
        src/fragment/writable_fragment.cpp
        include/fragment/writable_fragment.h
        src/fragment/fragment_iterator.cpp
)

add_executable(MemoryBucket ${SOURCES})

target_include_directories(MemoryBucket PRIVATE "./include/")
