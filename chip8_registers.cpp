#include "chip8_registers.h"
#include "ui_chip8_registers.h"

Chip8Registers::Chip8Registers(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chip8Registers)
{
    ui->setupUi(this);

    ui->registersTable->setRowCount(21U);
    QStringList registerNames;
    for (int i = 0U; i < 16U; ++i)
    {
        registerNames << QString("V%1").arg(i, 1U, 16U);
    }
    registerNames << "I" << "PC" << "SP" << "Delay" << "Sound";

    for (int i = 0U; i < 21U; ++i)
    {
        ui->registersTable->setItem(i, 0, new QTableWidgetItem(registerNames[i]));
    }
}

Chip8Registers::~Chip8Registers(void)
{
    delete ui;
}

void Chip8Registers::update_registers(const std::array<uint8_t, 16>& v, uint16_t i, uint16_t pc, uint8_t sp, uint8_t delay, uint8_t sound)
{
    for (int i = 0U; i < 16U; ++i)
    {
        ui->registersTable->setItem(i, 1, new QTableWidgetItem(QString("0x%1").arg(v[i], 2, 16, QChar('0'))));
    }
    ui->registersTable->setItem(16U, 1, new QTableWidgetItem(QString("0x%1").arg(i, 4U, 16U, QChar('0'))));
    ui->registersTable->setItem(17U, 1, new QTableWidgetItem(QString("0x%1").arg(pc, 4U, 16U, QChar('0'))));
    ui->registersTable->setItem(18U, 1, new QTableWidgetItem(QString("0x%1").arg(sp, 2U, 16U, QChar('0'))));
    ui->registersTable->setItem(19U, 1, new QTableWidgetItem(QString("0x%1").arg(delay, 2U, 16U, QChar('0'))));
    ui->registersTable->setItem(20U, 1, new QTableWidgetItem(QString("0x%1").arg(sound, 2U, 16U, QChar('0'))));
}
