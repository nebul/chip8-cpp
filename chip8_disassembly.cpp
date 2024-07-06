#include "chip8_disassembly.h"
#include "ui_chip8_disassembly.h"

Chip8Disassembly::Chip8Disassembly(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chip8Disassembly)
{
    ui->setupUi(this);
}

Chip8Disassembly::~Chip8Disassembly(void)
{
    delete ui;
}

void Chip8Disassembly::update_disassembly(const std::array<uint8_t, 4096>& memory, uint16_t pc)
{
    ui->disassemblyList->clear();
    for (uint16_t addr = 0x200U; addr < 0x1000U; addr += 2U)
    {
        uint16_t opcode = (memory[addr] << 8U) | memory[addr + 1U];
        QString line = QString("%1: %2").arg(addr, 4U, 16U, QChar('0')).arg(disassemble_opcode(opcode));
        QListWidgetItem* item = new QListWidgetItem(line);
        if (addr == pc) 
        {
            item->setBackground(Qt::yellow);
        }
        ui->disassemblyList->addItem(item);
    }
}

QString Chip8Disassembly::disassemble_opcode(uint16_t opcode)
{
    uint8_t x = (opcode & 0x0F00U) >> 8U;
    uint8_t y = (opcode & 0x00F0U) >> 4U;
    uint8_t n = opcode & 0x000FU;
    uint8_t nn = opcode & 0x00FFU;
    uint16_t nnn = opcode & 0x0FFFU;

    switch (opcode & 0xF000U)
    {
        case 0x0000U:
            switch (opcode) 
            {
                case 0x00E0U: return "CLS";
                case 0x00EEU: return "RET";
                default: return QString("SYS 0x%1").arg(nnn, 3U, 16U, QChar('0'));
            }
        case 0x1000U: return QString("JP 0x%1").arg(nnn, 3U, 16U, QChar('0'));
        case 0x2000U: return QString("CALL 0x%1").arg(nnn, 3U, 16U, QChar('0'));
        case 0x3000U: return QString("SE V%1, 0x%2").arg(x, 1U, 16U).arg(nn, 2U, 16U, QChar('0'));
        case 0x4000U: return QString("SNE V%1, 0x%2").arg(x, 1U, 16U).arg(nn, 2U, 16U, QChar('0'));
        case 0x5000U: return QString("SE V%1, V%2").arg(x, 1U, 16U).arg(y, 1U, 16U);
        case 0x6000U: return QString("LD V%1, 0x%2").arg(x, 1U, 16U).arg(nn, 2U, 16U, QChar('0'));
        case 0x7000U: return QString("ADD V%1, 0x%2").arg(x, 1U, 16U).arg(nn, 2U, 16U, QChar('0'));
        case 0x8000U:
            switch (n) 
            {
                case 0x0U: return QString("LD V%1, V%2").arg(x, 1U, 16U).arg(y, 1U, 16U);
                case 0x1U: return QString("OR V%1, V%2").arg(x, 1U, 16U).arg(y, 1U, 16U);
                case 0x2U: return QString("AND V%1, V%2").arg(x, 1U, 16U).arg(y, 1U, 16U);
                case 0x3U: return QString("XOR V%1, V%2").arg(x, 1U, 16U).arg(y, 1U, 16U);
                case 0x4U: return QString("ADD V%1, V%2").arg(x, 1U, 16U).arg(y, 1U, 16U);
                case 0x5U: return QString("SUB V%1, V%2").arg(x, 1U, 16U).arg(y, 1U, 16U);
                case 0x6U: return QString("SHR V%1 {, V%2}").arg(x, 1U, 16U).arg(y, 1U, 16U);
                case 0x7U: return QString("SUBN V%1, V%2").arg(x, 1U, 16U).arg(y, 1U, 16U);
                case 0xEU: return QString("SHL V%1 {, V%2}").arg(x, 1U, 16U).arg(y, 1, 16U);
                default: return QString("Unknown opcode: 0x%1").arg(opcode, 4U, 16U, QChar('0'));
            }
        case 0x9000U: return QString("SNE V%1, V%2").arg(x, 1U, 16U).arg(y, 1U, 16U);
        case 0xA000U: return QString("LD I, 0x%1").arg(nnn, 3U, 16U, QChar('0'));
        case 0xB000U: return QString("JP V0, 0x%1").arg(nnn, 3U, 16U, QChar('0'));
        case 0xC000U: return QString("RND V%1, 0x%2").arg(x, 1U, 16U).arg(nn, 2U, 16U, QChar('0'));
        case 0xD000U: return QString("DRW V%1, V%2, %3").arg(x, 1U, 16U).arg(y, 1U, 16U).arg(n);
        case 0xE000U:
            switch (nn) 
            {
                case 0x9EU: return QString("SKP V%1").arg(x, 1U, 16U);
                case 0xA1U: return QString("SKNP V%1").arg(x, 1U, 16U);
                default: return QString("Unknown opcode: 0x%1").arg(opcode, 4U, 16U, QChar('0'));
            }
        case 0xF000U:
            switch (nn) 
            {
                case 0x07U: return QString("LD V%1, DT").arg(x, 1U, 16U);
                case 0x0AU: return QString("LD V%1, K").arg(x, 1U, 16U);
                case 0x15U: return QString("LD DT, V%1").arg(x, 1U, 16U);
                case 0x18U: return QString("LD ST, V%1").arg(x, 1U, 16U);
                case 0x1EU: return QString("ADD I, V%1").arg(x, 1U, 16U);
                case 0x29U: return QString("LD F, V%1").arg(x, 1U, 16U);
                case 0x33U: return QString("LD B, V%1").arg(x, 1U, 16U);
                case 0x55U: return QString("LD [I], V%1").arg(x, 1U, 16U);
                case 0x65U: return QString("LD V%1, [I]").arg(x, 1U, 16U);
                default: return QString("Unknown opcode: 0x%1").arg(opcode, 4U, 16U, QChar('0'));
            }
        default: return QString("Unknown opcode: 0x%1").arg(opcode, 4U, 16U, QChar('0'));
    }
}
