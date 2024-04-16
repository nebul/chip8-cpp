#ifndef CHIP8_UI_CHIP8_REGISTERS_WIDGET_HPP
#define CHIP8_UI_CHIP8_REGISTERS_WIDGET_HPP

#include <QWidget>

#include "core/chip8.hpp"

class QTableWidget;

namespace chip8 {
namespace ui {

/**
 * @brief Read-only table view of the CHIP-8 registers and internal counters.
 *
 * Displays the V0..VF general purpose registers along with the index register,
 * program counter, stack pointer and the two timers.
 */
class RegistersWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Builds the register table.
     * @param parent Owning widget, or @c nullptr.
     */
    explicit RegistersWidget(QWidget* parent = nullptr);

    /**
     * @brief Refreshes every cell from the current interpreter state.
     * @param cpu Interpreter to read the register values from.
     */
    void update_registers(const Chip8& cpu);

private:
    QTableWidget* table_;
};

} // namespace ui
} // namespace chip8

#endif // CHIP8_UI_CHIP8_REGISTERS_WIDGET_HPP
