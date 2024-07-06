#ifndef CHIP8_STACK_H
#define CHIP8_STACK_H

#include <QWidget>
#include <array>
#include <cstdint>

namespace Ui {
class Chip8Stack;
}

class Chip8Stack : public QWidget
{
    Q_OBJECT

public:
    explicit Chip8Stack(QWidget *parent = nullptr);
    ~Chip8Stack(void);

    void update_stack(const std::array<uint16_t, 16>& stack, uint8_t sp);

private:
    Ui::Chip8Stack *ui;
};

#endif // CHIP8_STACK_H
