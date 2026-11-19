# Data Structure and Algorithm

A study project for data structures and algorithms, implemented in C++.
It includes implementations of basic data structures, algorithms, and test files.

## Features

- Basic data structure implementations
- Basic algorithm implementations
- Header-only library design (`.h` + `.tpp`).
- Automated build and test system using CMake and CMake Presets.
- Memory safety ensured by AddressSanitizer (ASan) on Linux.
- Study notes in `note.md`

## Language

- C++
- C++17 or later

## Project Structure

```text
.
├── data_struct/
│   ├── include/
│   │   ├── groups.dox              # Doxygen group definitions
│   │   └── data_struct/            # DS implementations
│   └── tests/                      # Test files
├── CMakeLists.txt                  # Build configuration
├── CMakePresets.json               # Cross-platform build presets
├── Doxyfile                        # Doxygen documentation config
├── .gitignore
├── note.md                         # Study notes
└── README.md
```

DS: Array, List(Doubly linked list), Stack, Queue, BST (binary search tree)

## Getting Started

### Prerequisites

The project supports cross-platform building via CMake Presets. Below are the prerequisites for both Linux/WSL and Windows environments.

#### Linux / WSL (`wsl-gcc-debug` preset, recommanded)

- **GCC** (`g++`) compiler supporting C++17.
- **[CMake](https://cmake.org/)** (version 3.20 or higher).
- **[Ninja](https://ninja-build.org/)** build system (required by the preset).
- *(Optional)* **[Doxygen](https://www.doxygen.nl/)** and **[Graphviz](https://graphviz.org/)** for generating API documentation.

**Ubuntu/Debian quick install:**

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build doxygen graphviz
```

#### Windows (`win-mingw-debug` preset)

- **MinGW-w64 (GCC)** compiler. Recommended via [MSYS2](https://www.msys2.org/) (UCRT64 environment).
- **[CMake](https://cmake.org/)** (version 3.20 or higher).
- **MinGW Makefiles** generator (comes with CMake).
- *(Optional)* **[Doxygen](https://www.doxygen.nl/)** and **[Graphviz](https://graphviz.org/)** for documentation.

> **Note for Windows Users:**
> Please update the `CMAKE_C_COMPILER` and `CMAKE_CXX_COMPILER` paths in the `CMakePresets.json` file, `win-mingw-debug` preset, or simply replace them with `"gcc"` and `"g++"` if they are in your system's `PATH`.

---

### Compile and Run

#### On Linux / WSL

```bash
# 1. Configure
cmake --preset wsl-gcc-debug

# 2. Build
cmake --build --preset wsl-gcc-debug

# 3. Run tests (example: Array)
./out/build/wsl-gcc-debug/test_array
```

> Note: AddressSanitizer (ASan) is enabled by default in this preset to detect memory leaks.

#### On Windows (MinGW)

```powershell
# 1. Configure
cmake --preset win-mingw-debug

# 2. Build
cmake --build --preset win-mingw-debug

# 3. Run tests (example: Array)
.\out\build\win-mingw-debug\test_array.exe
```

> Note: ASan and other memory leak detectors are not set in this configuration.

## Notes

This is a personal study project and will be updated continuously.
