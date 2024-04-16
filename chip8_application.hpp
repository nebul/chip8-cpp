#include <QApplication>
#include "chip8_mainwindow.h"

class Chip8Application : public QApplication
{
public:
    Chip8Application(int &argc, char **argv);
    ~Chip8Application();

private:
    Chip8MainWindow *chip8_mainWindow_;

};
