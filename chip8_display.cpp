#include "chip8_display.h"
#include "ui_chip8_display.h"

#include <QPainter>

Chip8Display::Chip8Display(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Chip8Display)
{
    ui->setupUi(this);
    screen_.fill(0U, 64U * 32U);
}

Chip8Display::~Chip8Display(void)
{
    delete ui;
}

void Chip8Display::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(this->rect());

    unsigned int pixel_width = (this->width() / 64U);
    unsigned int pixel_height = (this->height() / 32U);

    painter.setBrush(Qt::white);

    for (unsigned int column = 0U; column < 32U; column++)
    {
        for (unsigned int row = 0U; row < 64U; row++)
        {
            if (screen_[column * 64U + row] != 0U)
            {
                painter.drawRect(row * pixel_width, column * pixel_height, pixel_width, pixel_height);
            }
        }
    }
}

void Chip8Display::update_screen(QVector<uint8_t>& screen)
{
    screen_ = screen;
    update();
}
