cmake_minimum_required(VERSION 3.16)

project(chip8 VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(CHIP8_BUILD_TESTS "Build the core unit tests" OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(chip8_core STATIC
    src/core/chip8.cpp
    src/core/chip8_disassembler.cpp
)
target_include_directories(chip8_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(UI_SOURCES
    src/ui/chip8_display_widget.cpp
    src/ui/chip8_main_window.cpp
    src/ui/chip8_disassembly_widget.cpp
    src/ui/chip8_memory_widget.cpp
    src/ui/chip8_registers_widget.cpp
    src/ui/chip8_stack_widget.cpp
    include/ui/chip8_display_widget.hpp
    include/ui/chip8_main_window.hpp
    include/ui/chip8_disassembly_widget.hpp
    include/ui/chip8_memory_widget.hpp
    include/ui/chip8_registers_widget.hpp
    include/ui/chip8_stack_widget.hpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(chip8
        src/main.cpp
        ${UI_SOURCES}
    )
else()
    add_executable(chip8
        src/main.cpp
        ${UI_SOURCES}
    )
endif()

target_link_libraries(chip8 PRIVATE chip8_core Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(chip8 PROPERTIES
    AUTOMOC ON
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.chip8
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

include(GNUInstallDirs)
install(TARGETS chip8
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(CHIP8_BUILD_TESTS)
    enable_testing()
    add_executable(chip8_core_tests tests/test_core.cpp)
    target_link_libraries(chip8_core_tests PRIVATE chip8_core)
    add_test(NAME chip8_core_tests COMMAND chip8_core_tests)
endif()
