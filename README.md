# CHIP-8 Emulator

A modern CHIP-8 emulator with a Qt graphical interface and built-in debugging tools.

## Features

- Complete emulation of the CHIP-8 system
- Modern Qt user interface, built entirely in code (no `.ui` files)
- Customisable display (colours, optional pixel grid)
- Decoupled CPU and 60 Hz timers for accurate timing
- Integrated debugging views:
  - Disassembler
  - Memory viewer
  - Register viewer
  - Stack viewer
- Step mode for instruction-level debugging
- Reusable, Qt-independent emulator core (`chip8_core` static library)

## Requirements

- A C++17 compiler
- Qt 5.15 or Qt 6.x (Widgets)
- CMake 3.16 or newer

## Building

```bash
cmake -S . -B build
cmake --build build
```

To also build the core unit tests:

```bash
cmake -S . -B build -DCHIP8_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build
```

## Usage

### Controls

The emulator maps the CHIP-8 keypad to the following keyboard layout:

```
PC keyboard  | CHIP-8
-------------|---------
1 2 3 4      | 1 2 3 C
Q W E R      | 4 5 6 D
A S D F      | 7 8 9 E
Z X C V      | A 0 B F
```

### Loading a program

1. Launch the emulator.
2. Open **File > Open ROM...**.
3. Select a CHIP-8 ROM file.

### Emulation and debugging

- **Emulation > Pause/Resume** toggles execution.
- **Emulation > Step** runs a single instruction while paused.
- **Emulation > Reset** restarts the loaded ROM.
- **View > Toggle Debug Views** shows or hides the register, memory, stack and
  disassembly docks.

## Project structure

```
chip8-cpp/
├── include/
│   ├── core/   # Emulator core headers (no Qt dependency)
│   └── ui/     # Qt widget headers
├── src/
│   ├── core/   # Emulator core implementation
│   ├── ui/     # Qt widget implementation
│   └── main.cpp
├── tests/      # Standalone core unit tests
└── CMakeLists.txt
```

## License

This project is licensed under the Apache 2.0 License. See the LICENSE file for details.
