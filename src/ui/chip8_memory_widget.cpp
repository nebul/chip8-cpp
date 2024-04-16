#include "ui/chip8_memory_widget.hpp"

#include <QHeaderView>
#include <QStringList>
#include <QTableWidget>
#include <QVBoxLayout>

namespace chip8 {
namespace ui {
namespace {

/// Number of bytes shown per table row.
constexpr int kColumnCount = 16;
/// Number of table rows required to cover the whole memory image.
constexpr int kRowCount = Chip8::MEMORY_SIZE / kColumnCount;

} // namespace

MemoryWidget::MemoryWidget(QWidget* parent)
    : QWidget(parent)
    , table_(new QTableWidget(kRowCount, kColumnCount, this)) {
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList columns;
    for (int col = 0; col < kColumnCount; ++col) {
        columns << QStringLiteral("%1").arg(col, 1, 16).toUpper();
    }
    table_->setHorizontalHeaderLabels(columns);
    for (int row = 0; row < kRowCount; ++row) {
        table_->setVerticalHeaderItem(
            row, new QTableWidgetItem(
                     QStringLiteral("%1").arg(row * kColumnCount, 3, 16, QChar('0'))));
    }

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(table_);
}

void MemoryWidget::update_memory(const Chip8& cpu) {
    const auto& memory = cpu.memory();
    for (int row = 0; row < kRowCount; ++row) {
        for (int col = 0; col < kColumnCount; ++col) {
            const int index = row * kColumnCount + col;
            table_->setItem(
                row, col,
                new QTableWidgetItem(
                    QStringLiteral("%1").arg(memory[index], 2, 16, QChar('0'))));
        }
    }
}

} // namespace ui
} // namespace chip8
