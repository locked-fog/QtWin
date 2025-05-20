# QtWin Project

A Qt6 library providing Windows-specific UI enhancements.

## Prerequisites
- CMake 3.16+
- Qt6 (Core, Gui, Widgets components)
- C++17 compatible compiler
- Windows SDK (for Windows builds)

## Building

```bash
# Configure
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release
```

## Installing

```bash
# From build directory
cmake --install . --prefix "your/install/path"
```

This will install:
- Library files to `library/`
- Headers to `include/`
- CMake config files to `library/cmake/QtWin/`

## Usage in Other Projects

After installation, you can use find_package() in your CMake projects:

```cmake
find_package(QtWin REQUIRED)
target_link_libraries(your_target PRIVATE QtWin::QtWin)
```

## Features

- `qtwDWM`: Windows DWM (Desktop Window Manager) integration
- `qtwButton`: Enhanced Qt buttons with Windows styling
- `qtwMonet`: Windows-themed UI components
- `qtwException`: Windows-specific exception handling

## Notes for Windows Development

The library automatically links against `dwmapi.lib` on Windows builds.
