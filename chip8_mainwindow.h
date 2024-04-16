#ifndef CHIP8_MAINWINDOW_H
#define CHIP8_MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "chip8_display.h"

#include "chip8.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class Chip8MainWindow;
}
QT_END_NAMESPACE

class Chip8MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Chip8MainWindow(QWidget *parent = nullptr);
    ~Chip8MainWindow();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void update_key_state(QKeyEvent *event, bool state);

private slots:
    void load_program();
    void run_cycle();

private:
    Ui::Chip8MainWindow *ui;
    QTimer * timer_;
    Chip8Display * chip8_display_;
    Chip8 chip8_;
    bool chip8_state_;
};
#endif // CHIP8_MAINWINDOW_H
