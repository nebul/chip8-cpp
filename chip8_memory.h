#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include <QWidget>

namespace Ui {
class Chip8Memory;
}

class Chip8Memory : public QWidget
{
    Q_OBJECT

public:
    explicit Chip8Memory(QWidget *parent = nullptr);
    ~Chip8Memory();

private:
    Ui::Chip8Memory *ui;
};

#endif // CHIP8_MEMORY_H
