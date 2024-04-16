#include <QApplication>

#include "ui/chip8_main_window.hpp"

/**
 * @brief Application entry point.
 *
 * Initialises the Qt application and shows the emulator main window.
 *
 * @param argc Argument count.
 * @param argv Argument values.
 * @return Qt event loop exit code.
 */
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QApplication::setApplicationName("CHIP-8 Emulator");
    QApplication::setApplicationVersion("0.2.0");

    chip8::ui::MainWindow window;
    window.show();

    return app.exec();
}
