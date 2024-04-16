#ifndef CHIP8_DISASSEMBLY_H
#define CHIP8_DISASSEMBLY_H

#include <QWidget>

namespace Ui {
class Chip8Disassembly;
}

class Chip8Disassembly : public QWidget
{
    Q_OBJECT

public:
    explicit Chip8Disassembly(QWidget *parent = nullptr);
    ~Chip8Disassembly();

private:
    Ui::Chip8Disassembly *ui;
};

#endif // CHIP8_DISASSEMBLY_H
