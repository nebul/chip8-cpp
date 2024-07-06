#ifndef CHIP8_DISASSEMBLY_H
#define CHIP8_DISASSEMBLY_H

#include <QWidget>
#include <array>
#include <cstdint>

namespace Ui {
class Chip8Disassembly;
}

class Chip8Disassembly : public QWidget
{
    Q_OBJECT

public:
    explicit Chip8Disassembly(QWidget *parent = nullptr);
    ~Chip8Disassembly(void);

    void update_disassembly(const std::array<uint8_t, 4096>& memory, uint16_t pc);

private:
    Ui::Chip8Disassembly *ui;

    QString disassemble_opcode(uint16_t opcode);
};

#endif // CHIP8_DISASSEMBLY_H
