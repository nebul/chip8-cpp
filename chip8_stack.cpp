#include "chip8_stack.h"
#include "ui_chip8_stack.h"

Chip8Stack::Chip8Stack(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chip8Stack)
{
    ui->setupUi(this);
}

Chip8Stack::~Chip8Stack()
{
    delete ui;
}
