#include "ui/chip8_stack_widget.hpp"

#include <QListWidget>
#include <QVBoxLayout>

namespace chip8 {
namespace ui {

StackWidget::StackWidget(QWidget* parent)
    : QWidget(parent)
    , list_(new QListWidget(this)) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(list_);
}

void StackWidget::update_stack(const Chip8& cpu) {
    const auto& stack = cpu.stack();
    const std::uint8_t sp = cpu.stack_pointer();

    list_->clear();
    for (std::size_t i = 0; i < stack.size(); ++i) {
        const QString text =
            QStringLiteral("%1: 0x%2").arg(i).arg(stack[i], 4, 16, QChar('0'));
        auto* item = new QListWidgetItem(text, list_);
        if (i == static_cast<std::size_t>(sp)) {
            item->setBackground(Qt::yellow);
        }
    }
}

} // namespace ui
} // namespace chip8
