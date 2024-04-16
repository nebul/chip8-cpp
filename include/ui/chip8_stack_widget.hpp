#ifndef CHIP8_UI_CHIP8_STACK_WIDGET_HPP
#define CHIP8_UI_CHIP8_STACK_WIDGET_HPP

#include <QWidget>

#include "core/chip8.hpp"

class QListWidget;

namespace chip8 {
namespace ui {

/**
 * @brief List view of the CHIP-8 call stack.
 *
 * Each entry shows a stack slot and its value; the slot pointed at by the
 * stack pointer is highlighted.
 */
class StackWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Builds the stack list.
     * @param parent Owning widget, or @c nullptr.
     */
    explicit StackWidget(QWidget* parent = nullptr);

    /**
     * @brief Refreshes the list from the current interpreter state.
     * @param cpu Interpreter to read the stack and stack pointer from.
     */
    void update_stack(const Chip8& cpu);

private:
    QListWidget* list_;
};

} // namespace ui
} // namespace chip8

#endif // CHIP8_UI_CHIP8_STACK_WIDGET_HPP
