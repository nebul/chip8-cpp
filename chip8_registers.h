#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H

#include <QWidget>
#include <array>
#include <cstdint>

namespace Ui {
class Chip8Registers;
}

class Chip8Registers : public QWidget
{
    Q_OBJECT

public:
    explicit Chip8Registers(QWidget *parent = nullptr);
    ~Chip8Registers(void);

    void update_registers(const std::array<uint8_t, 16>& v, uint16_t i, uint16_t pc, uint8_t sp, uint8_t delay, uint8_t sound);

private:
    Ui::Chip8Registers *ui;
};

#endif // CHIP8_REGISTERS_H
