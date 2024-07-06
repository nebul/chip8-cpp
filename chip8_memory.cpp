#include "chip8_memory.h"
#include "ui_chip8_memory.h"

Chip8Memory::Chip8Memory(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chip8Memory)
{
    ui->setupUi(this);

    ui->memoryTable->setRowCount(256U);
    ui->memoryTable->setHorizontalHeaderLabels(QStringList() << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "A" << "B" << "C" << "D" << "E" << "F");
    for (int i = 0U; i < 256U; ++i)
    {
        ui->memoryTable->setVerticalHeaderItem(i, new QTableWidgetItem(QString("%1").arg(i * 16U, 3U, 16U, QChar('0'))));
    }
}

Chip8Memory::~Chip8Memory(void)
{
    delete ui;
}

void Chip8Memory::update_memory(const std::array<uint8_t, 4096U>& memory)
{
    for (int row = 0U; row < 256U; ++row)
     {
        for (int col = 0U; col < 16U; ++col)
         {
            int index = row * 16U + col;
            ui->memoryTable->setItem(row, col, new QTableWidgetItem(QString("%1").arg(memory[index], 2U, 16U, QChar('0'))));
        }
    }
}
