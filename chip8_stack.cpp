#include "chip8_stack.h"
#include "ui_chip8_stack.h"

Chip8Stack::Chip8Stack(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chip8Stack)
{
    ui->setupUi(this);
}

Chip8Stack::~Chip8Stack(void)
{
    delete ui;
}

void Chip8Stack::update_stack(const std::array<uint16_t, 16>& stack, uint8_t sp)
{
    ui->stackList->clear();
    for (int i = 0U; i < 16U; ++i)
    {
        QString item = QString("%1: 0x%2").arg(i).arg(stack[i], 4U, 16U, QChar('0'));
        QListWidgetItem* listItem = new QListWidgetItem(item);
        if (i == sp) 
        {
            listItem->setBackground(Qt::yellow);
        }
        ui->stackList->addItem(listItem);
    }
}
