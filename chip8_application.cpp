#include "chip8_application.hpp"

Chip8Application::Chip8Application(int &argc, char **argv) : QApplication(argc, argv) 
{
    chip8_mainWindow_ = new Chip8MainWindow();
    chip8_mainWindow_->show();
}

Chip8Application::~Chip8Application() 
{
    delete chip8_mainWindow_;
}
