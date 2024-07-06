#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include <QWidget>
#include <array>
#include <cstdint>

namespace Ui {
class Chip8Memory;
}

class Chip8Memory : public QWidget
{
    Q_OBJECT

public:
    explicit Chip8Memory(QWidget *parent = nullptr);
    ~Chip8Memory(void);

    void update_memory(const std::array<uint8_t, 4096>& memory);

private:
    Ui::Chip8Memory *ui;
};

#endif // CHIP8_MEMORY_H
