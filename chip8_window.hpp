#include <QMainWindow>
#include <QTimer>

#include "chip8.hpp"
#include "chip8_display.hpp"

class Chip8Window : public QWMainWindow
{
Q_OBJECT

public:
    explicit Chip8Window(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Chip8 chip8;
    Chip8Display chip8_display;
};  
