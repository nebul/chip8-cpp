#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H

#include <QWidget>

namespace Ui {
class Chip8Registers;
}

class Chip8Registers : public QWidget
{
    Q_OBJECT

public:
    explicit Chip8Registers(QWidget *parent = nullptr);
    ~Chip8Registers();

private:
    Ui::Chip8Registers *ui;
};

#endif // CHIP8_REGISTERS_H
