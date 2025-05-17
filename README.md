# QtWin - Windows Qt Application

A Qt-based application for Windows with DWM (Desktop Window Manager) integration.

## Features
- Windows-specific Qt integration
- DWM (Desktop Window Manager) support
- Custom exception handling

## Build Requirements
- CMake 3.16+
- Qt 6 (Core, Gui, Widgets modules required)
- C++17 compatible compiler
- Windows SDK (for DWM integration)

## Building
```bash
# Configure with CMake
cmake -B build -S .

# Build the project
cmake --build build --config Release

# Install the library (optional)
cmake --install build --config Release
```

## Installation
The project can be installed system-wide with:
```bash
cmake --install build --config Release
```

This will install:
- Runtime components to `library/`
- Development headers to `include/`
- CMake config files to `library/cmake/QtWin/`

## Usage
To use the library in another CMake project:
```cmake
find_package(QtWin REQUIRED)
target_link_libraries(your_target PRIVATE QtWin::QtWin)
```

For standalone usage, link against the library:
```bash
./build/Release/QtWin.exe
```

## License
MIT License - See [LICENSE](LICENSE) file for details
