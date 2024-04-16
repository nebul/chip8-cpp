#include "chip8_application.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    Chip8Application chip8_application = Chip8Application(argc, argv);

    return chip8_application.exec();
}
