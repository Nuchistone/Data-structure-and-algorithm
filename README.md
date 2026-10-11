# Data Structure and Algorithm

A study project for data structures and algorithms, implemented in C++.
It includes implementations of basic data structures, algorithms, and test files.

## Features

- Basic data structure implementations
- Basic algorithm implementations
- Test files for verification
- Study notes in `note.md`

## Language

- C++
- C++14 or later

## Project Structure

```text
.
├── data_struct/
│   └── include/
│       └── data_struct/            # DS implementations
├── tests/                          # Test files
├── .gitignore
├── note.md                         # Study notes
└── README.md
```

## Getting Started

### Prerequisites

- A C++ compiler such as `g++`, `clang++`, or MSVC
- C++14 or later

### Compile and Run

There is no build system yet, so you can compile the test manually.

For example, to compile and run `test_array.cpp`:

```bash
g++ -std=c++14 -I data_struct/include tests/test_array.cpp -o output/test_array
./output/test_array
```

On Windows, the output file may be `output/test_array.exe`.

## Notes

This is a personal study project and will be updated continuously.