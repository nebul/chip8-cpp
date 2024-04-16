#include "ui/chip8_registers_widget.hpp"

#include <QHeaderView>
#include <QStringList>
#include <QTableWidget>
#include <QVBoxLayout>

namespace chip8 {
namespace ui {
namespace {

/// Number of table rows: 16 general registers plus six special values.
constexpr int kRowCount = 21;

/**
 * @brief Formats a value as a fixed-width, zero-padded hexadecimal string.
 * @param value Value to format.
 * @param digits Number of hexadecimal digits to pad to.
 * @return String of the form "0x0a".
 */
QString to_hex(unsigned int value, int digits) {
    return QStringLiteral("0x%1").arg(value, digits, 16, QChar('0'));
}

} // namespace

RegistersWidget::RegistersWidget(QWidget* parent)
    : QWidget(parent)
    , table_(new QTableWidget(kRowCount, 2, this)) {
    table_->setHorizontalHeaderLabels(QStringList() << "Register" << "Value");
    table_->verticalHeader()->setVisible(false);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->horizontalHeader()->setStretchLastSection(true);

    QStringList names;
    for (int i = 0; i < 16; ++i) {
        names << QStringLiteral("V%1").arg(i, 1, 16);
    }
    names << "I" << "PC" << "SP" << "Delay" << "Sound";
    for (int row = 0; row < kRowCount; ++row) {
        table_->setItem(row, 0, new QTableWidgetItem(names[row]));
    }

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(table_);
}

void RegistersWidget::update_registers(const Chip8& cpu) {
    const auto& v = cpu.registers();
    for (int i = 0; i < 16; ++i) {
        table_->setItem(i, 1, new QTableWidgetItem(to_hex(v[i], 2)));
    }
    table_->setItem(16, 1, new QTableWidgetItem(to_hex(cpu.index_register(), 4)));
    table_->setItem(17, 1, new QTableWidgetItem(to_hex(cpu.program_counter(), 4)));
    table_->setItem(18, 1, new QTableWidgetItem(to_hex(cpu.stack_pointer(), 2)));
    table_->setItem(19, 1, new QTableWidgetItem(to_hex(cpu.delay_timer(), 2)));
    table_->setItem(20, 1, new QTableWidgetItem(to_hex(cpu.sound_timer(), 2)));
}

} // namespace ui
} // namespace chip8
