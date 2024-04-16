#include "ui/chip8_disassembly_widget.hpp"

#include <QListWidget>
#include <QString>
#include <QVBoxLayout>

#include "core/chip8_disassembler.hpp"

namespace chip8 {
namespace ui {

DisassemblyWidget::DisassemblyWidget(QWidget* parent)
    : QWidget(parent)
    , list_(new QListWidget(this)) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(list_);
}

void DisassemblyWidget::update_disassembly(const Chip8& cpu) {
    const auto lines = Disassembler::disassemble_memory(
        cpu.memory(), Chip8::PROGRAM_START_ADDRESS, Chip8::MEMORY_SIZE);
    const std::uint16_t pc = cpu.program_counter();

    list_->clear();
    QListWidgetItem* current = nullptr;
    for (const auto& [address, mnemonic] : lines) {
        const QString text = QStringLiteral("%1: %2")
                                 .arg(address, 4, 16, QChar('0'))
                                 .arg(QString::fromStdString(mnemonic));
        auto* item = new QListWidgetItem(text, list_);
        if (address == pc) {
            item->setBackground(Qt::yellow);
            current = item;
        }
    }

    if (current != nullptr) {
        list_->scrollToItem(current);
    }
}

} // namespace ui
} // namespace chip8
