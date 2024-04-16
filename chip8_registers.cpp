#include "chip8_registers.h"
#include "ui_chip8_registers.h"

Chip8Registers::Chip8Registers(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chip8Registers)
{
    ui->setupUi(this);
}

Chip8Registers::~Chip8Registers()
{
    delete ui;
}
