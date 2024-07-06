#ifndef CHIP8MAINWINDOW_H
#define CHIP8MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "chip8_display.h"
#include "chip8_disassembly.h"
#include "chip8_memory.h"
#include "chip8_registers.h"
#include "chip8_stack.h"
#include "chip8.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class Chip8MainWindow; }
QT_END_NAMESPACE

class Chip8MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Chip8MainWindow(QWidget *parent = nullptr);
    ~Chip8MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void load_program(void);
    void run_cycle(void);
    void toggleDebugView(void);

private:
    void update_key_state(QKeyEvent *event, bool state);
    void update_debug_widgets(void);
    void setup_debug_widgets(void);

    Ui::Chip8MainWindow *ui;
    QTimer *timer_;
    Chip8Display *chip8_display_;
    Chip8Disassembly *chip8_disassembly_;
    Chip8Memory *chip8_memory_;
    Chip8Registers *chip8_registers_;
    Chip8Stack *chip8_stack_;
    Chip8 chip8_;
    bool chip8_state_;
    bool debug_view_active_;
};
#endif // CHIP8MAINWINDOW_H
