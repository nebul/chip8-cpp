#ifndef CHIP8_UI_CHIP8_MEMORY_WIDGET_HPP
#define CHIP8_UI_CHIP8_MEMORY_WIDGET_HPP

#include <QWidget>

#include "core/chip8.hpp"

class QTableWidget;

namespace chip8 {
namespace ui {

/**
 * @brief Hexadecimal dump of the full CHIP-8 memory.
 *
 * Memory is laid out as 256 rows of 16 bytes; row and column headers give the
 * base address of each cell.
 */
class MemoryWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Builds the memory table and its headers.
     * @param parent Owning widget, or @c nullptr.
     */
    explicit MemoryWidget(QWidget* parent = nullptr);

    /**
     * @brief Refreshes every cell from the current interpreter memory.
     * @param cpu Interpreter to read the memory image from.
     */
    void update_memory(const Chip8& cpu);

private:
    QTableWidget* table_;
};

} // namespace ui
} // namespace chip8

#endif // CHIP8_UI_CHIP8_MEMORY_WIDGET_HPP
