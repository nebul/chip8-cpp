#include "chip8_disassembly.h"
#include "ui_chip8_disassembly.h"

Chip8Disassembly::Chip8Disassembly(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chip8Disassembly)
{
    ui->setupUi(this);
}

Chip8Disassembly::~Chip8Disassembly()
{
    delete ui;
}
