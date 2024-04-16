#ifndef CHIP8_STACK_H
#define CHIP8_STACK_H

#include <QWidget>

namespace Ui {
class Chip8Stack;
}

class Chip8Stack : public QWidget
{
    Q_OBJECT

public:
    explicit Chip8Stack(QWidget *parent = nullptr);
    ~Chip8Stack();

private:
    Ui::Chip8Stack *ui;
};

#endif // CHIP8_STACK_H
