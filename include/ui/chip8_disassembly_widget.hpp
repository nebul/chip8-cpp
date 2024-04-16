#ifndef CHIP8_UI_CHIP8_DISASSEMBLY_WIDGET_HPP
#define CHIP8_UI_CHIP8_DISASSEMBLY_WIDGET_HPP

#include <QWidget>

#include "core/chip8.hpp"

class QListWidget;

namespace chip8 {
namespace ui {

/**
 * @brief List view of the disassembled program memory.
 *
 * Uses @ref chip8::Disassembler to render every two-byte instruction from the
 * program area and highlights the instruction at the program counter.
 */
class DisassemblyWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Builds the disassembly list.
     * @param parent Owning widget, or @c nullptr.
     */
    explicit DisassemblyWidget(QWidget* parent = nullptr);

    /**
     * @brief Rebuilds the listing and highlights the current instruction.
     * @param cpu Interpreter to read the memory and program counter from.
     */
    void update_disassembly(const Chip8& cpu);

private:
    QListWidget* list_;
};

} // namespace ui
} // namespace chip8

#endif // CHIP8_UI_CHIP8_DISASSEMBLY_WIDGET_HPP
