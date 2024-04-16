#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include <QWidget>

namespace Ui {
class Chip8Display;
}

class Chip8Display : public QWidget
{
    Q_OBJECT

public:
    explicit Chip8Display(QWidget *parent = nullptr);
    ~Chip8Display();

    void update_screen(QVector<uint8_t>& screen);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<uint8_t> screen_;

private:
    Ui::Chip8Display *ui;
};

#endif // CHIP8_DISPLAY_H


