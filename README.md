# VectorLibrary - Dynamic Arrays in Pure C

A library for working with dynamic arrays (vectors) in C, supporting various data types through macros.

## Contents
- [Features](#-features)
- [Requirements](#-requirements)
- [Building](#-building)
  - [Building with CMake](#building-with-cmake)
  - [Building with Make](#building-with-make)
- [Type Generator](#-type-generator)
- [Usage](#-usage)
- [Build Options](#-build-options)
- [Installation](#-installation)
- [Testing](#-testing)
- [Project Structure](#-project-structure)

## Features

- Support for any data types (built-in, custom, pointers)
- Lite version without function pointers (great for embedded systems)
- Memory size optimization
- Runtime checks (optional)
- Cross-platform (Windows, Linux, macOS)
- Build with CMake and Make

## Requirements

### For building:
- **CMake** (version 3.21 or higher) or **Make**
- **C compiler** (GCC, Clang, MSVC)
- **Python 3** (for type generator, optional)

### Supported platforms:
- Windows (MSVC, MinGW, Cygwin)
- Linux (GCC, Clang)
- macOS (Clang, GCC)
- BSD systems

## Building

### Type Initialization

Required data types must be added in the src/vector.h file before building:
```c
#include "vector_initialize_type.h"


VECTOR_INITIALIZE_TYPE(int)
VECTOR_INITIALIZE_TYPE(char)
VECTOR_INITIALIZE_TYPE(double *)

struct Point
{
    int x;
    int y;
};

VECTOR_INITIALIZE_TYPE(struct Point)
VECTOR_INITIALIZE_TYPE(struct Point *)


#include "vector_decl.h"
```

### Building with CMake

```bash
# Clone the repository
git clone https://github.com/Sasha7291/VectorLibrary.git
cd VectorLibrary

# Create build directory
mkdir build && cd build

# Configuration
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build .

# Build with options
cmake .. -DVECTOR_CHECK_ON=ON -DVECTOR_BUILD_TESTS=ON
cmake --build .
```

### Building with Make

```bash
# Simple build
make

# Build with options
make VECTOR_CHECK_ON=ON VECTOR_OPTIMIZE_SIZE=ON

# Build tests
make VECTOR_BUILD_TESTS=ON test

# Configuration info
make info

# Clean
make clean
make clean_gen  # clean generated files
```

## Type Generator

The library uses a generator to create specialized versions for each type. The generator is automatically selected in the following order:

1. **Python** (`script/vector_gen.py`) - preferred option
2. **Batch** (`script/vector_gen.bat`) - for Windows without Python
3. **Bash** (`script/vector_gen.sh`) - for Unix systems without Python

### Manual generator execution:

```bash
# Python
python script/vector_gen.py

# Batch (Windows)
script\vector_gen.bat

# Bash (Linux/macOS)
chmod +x script/vector_gen.sh
./script/vector_gen.sh
```

## Usage

### Library usage examples:

1. With VECTOR_LITE=OFF

```c
#include "vector.h"

int main() {
    // Create vector for int
    vector_int_t *vec = vector_create_int_t();
    
    // Add elements
    vector->push_back(&vec, 10);
    vector->push_back(&vec, 20);
    vector->push_back(&vec, 30);
    
    // Element access
    const int first = vector->at(&vec, 0);
    const int last = vector->back(&vec);
    
    // Size and capacity
    const size_t size = vector->size(&vec);
    const size_t capacity = vector->capacity(&vec);
    
    // Free memory
    destroy_vector_int_t(&vec);
    
    return 0;
}
```

2. With VECTOR_LITE=ON

```c
#include "vector.h"

int main() {
    // Create vector for int
    vector_int_t *vec = vector_create_int_t();
    
    // Add elements
    vector_push_back_int_t(&vec, 10);
    vector_push_back_int_t(&vec, 20);
    vector_push_back_int_t(&vec, 30);
    
    // Element access
    int first = vector_at_int_t(&vec, 0);
    int last = vector_back_int_t(&vec);
    
    // Size and capacity
    size_t size = vector_size_int_t(&vec);
    size_t capacity = vector_capacity_int_t(&vec);
    
    // Free memory
    destroy_vector_int_t(&vec);
    
    return 0;
}
```

## Build Options

### CMake Options:
| Option | Description | Default |
|--------|-------------|---------|
| `VECTOR_CHECK_ON` | Enable runtime checks | `OFF` |
| `VECTOR_LITE` | Lite version (without function pointers) | `OFF` |
| `VECTOR_OPTIMIZE_SIZE` | Optimize for size | `OFF` |
| `VECTOR_SMALL_MEMORY` | Small initial capacity | `OFF` |
| `VECTOR_BUILD_TESTS` | Build tests | `OFF` |
| `VECTOR_RUN_GENERATOR` | Run type generator | `ON` |

### Make Options:
```bash
make VECTOR_CHECK_ON=ON VECTOR_LITE=ON
make VECTOR_OPTIMIZE_SIZE=ON VECTOR_SMALL_MEMORY=ON
make prefix=/usr/local install
```

## Installation

### CMake:
```bash
# Install to system
sudo cmake --install .

# Install to different directory
cmake --install . --prefix /opt/vectorlib
```

### Make:
```bash
# Install to /usr/local
sudo make install

# Install to different directory
make install prefix=/opt/vectorlib

# Install with DESTDIR (for packaging)
make install DESTDIR=/tmp/package-build prefix=/usr
```

### Uninstallation:
```bash
# CMake
sudo cmake --build . --target uninstall

# Make
sudo make uninstall
```

## Testing

### Running tests:

```bash
# CMake
cmake .. -DVECTOR_BUILD_TESTS=ON
cmake --build .
ctest

# Make
make VECTOR_BUILD_TESTS=ON test
```

## Project Structure

```
VectorLibrary/
├── CMakeLists.txt            # Main CMake file
├── Makefile.mak              # Makefile for building
├── README.md                 # Documentation
├── script/                   # Generator scripts
│   ├── vector_gen.py         # Python generator
│   ├── vector_gen.bat        # Windows generator
│   └── vector_gen.sh         # Unix generator
├── src/                      # Source code
│   ├── vector.h              # Main header
│   ├── vector.c              # Implementation
│   ├── priv/                 # Private headers
│   │   ├── template.h
│   │   ├── vector_template.h
│   │   └── vector_template.c
│   ├── gen/                   # Generated files
│   └── test/                  # Tests
├── include/                   # Installable headers
├── doc/                       # Documentation
└── log/                       # Generator logs
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Contact

- Author: Sasha7291
- Email: sasha.terentev43@gmail.com
- GitHub: [@Sasha7291](https://github.com/Sasha7291)
