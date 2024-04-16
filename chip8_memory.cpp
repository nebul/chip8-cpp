#include "chip8_memory.h"
#include "ui_chip8_memory.h"

Chip8Memory::Chip8Memory(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chip8Memory)
{
    ui->setupUi(this);
}

Chip8Memory::~Chip8Memory()
{
    delete ui;
}
