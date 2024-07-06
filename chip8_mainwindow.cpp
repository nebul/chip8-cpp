#include "chip8_mainwindow.h"
#include "ui_chip8_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>

Chip8MainWindow::Chip8MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Chip8MainWindow)
{
    ui->setupUi(this);

    chip8_state_ = false;
    debug_view_active_ = false;

    chip8_display_ = new Chip8Display(this);
    setCentralWidget(chip8_display_);

    setup_debug_widgets();

    QAction *load_program_action = ui->menubar->addAction("Load Program");
    connect(load_program_action, &QAction::triggered, this, &Chip8MainWindow::load_program);

    QAction *toggle_debug_action = ui->menubar->addAction("Toggle Debug View");
    connect(toggle_debug_action, &QAction::triggered, this, &Chip8MainWindow::toggleDebugView);

    QAction *exit_action = ui->menubar->addAction("Exit");
    connect(exit_action, &QAction::triggered, this, &Chip8MainWindow::close);

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &Chip8MainWindow::run_cycle);
    timer_->setInterval(2);
    timer_->start();
}

Chip8MainWindow::~Chip8MainWindow(void)
{
    delete ui;
    timer_->stop();
}

void Chip8MainWindow::setup_debug_widgets()
{
    chip8_disassembly_ = new Chip8Disassembly(this);
    chip8_memory_ = new Chip8Memory(this);
    chip8_registers_ = new Chip8Registers(this);
    chip8_stack_ = new Chip8Stack(this);

    ui->disassemblyDock->setWidget(chip8_disassembly_);
    ui->memoryDock->setWidget(chip8_memory_);
    ui->registersDock->setWidget(chip8_registers_);
    ui->stackDock->setWidget(chip8_stack_);

    ui->disassemblyDock->hide();
    ui->memoryDock->hide();
    ui->registersDock->hide();
    ui->stackDock->hide();
}

void Chip8MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        update_key_state(event, true);
    }
}

void Chip8MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        update_key_state(event, false);
    }
}


void Chip8MainWindow::update_key_state(QKeyEvent *event, bool state)
{
    uint8_t key = 0xFFU;

    switch (event->key())
    {
        case Qt::Key_1: key = 0x1U; break;
        case Qt::Key_2: key = 0x2U; break;
        case Qt::Key_3: key = 0x3U; break;
        case Qt::Key_4: key = 0xCU; break;
        case Qt::Key_Q: key = 0x4U; break;
        case Qt::Key_W: key = 0x5U; break;
        case Qt::Key_E: key = 0x6U; break;
        case Qt::Key_R: key = 0xDU; break;
        case Qt::Key_A: key = 0x7U; break;
        case Qt::Key_S: key = 0x8U; break;
        case Qt::Key_D: key = 0x9U; break;
        case Qt::Key_F: key = 0xEU; break;
        case Qt::Key_Z: key = 0xAU; break;
        case Qt::Key_X: key = 0x0U; break;
        case Qt::Key_C: key = 0xBU; break;
        case Qt::Key_V: key = 0xFU; break;
        default: return;
    }
    chip8_.set_key_state(key, state);
}

void Chip8MainWindow::load_program()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Load file", "",
                                                    "*.*");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray fileContent = file.readAll();
            file.close();

            std::vector<uint8_t> program_data(fileContent.begin(), fileContent.end());

            chip8_.initialize();
            chip8_.load_program(program_data);
            chip8_state_ = true;
        }
        else
        {
            QMessageBox::warning(this, "Error", "Unable to open the file.");
        }
    }
}

void Chip8MainWindow::run_cycle()
{
    if(chip8_state_)
    {
        chip8_.run_cycle();
        bool beep_state = chip8_.get_beep_state();

        std::array<uint8_t, 64U * 32U> display_state = chip8_.get_display_state();
        QVector<uint8_t> screen_state = QVector<uint8_t>(display_state.begin(), display_state.end());

        chip8_display_->update_screen(screen_state);

        if(beep_state)
        {
            QApplication::beep();
        }

        if (debug_view_active_)
        {
            setup_debug_widgets();
        }
    }
}

void Chip8MainWindow::toggleDebugView()
{
    debug_view_active_ = !debug_view_active_;
    if (debug_view_active_)
    {
        ui->disassemblyDock->show();
        ui->memoryDock->show();
        ui->registersDock->show();
        ui->stackDock->show();
        setup_debug_widgets();
    }
    else
    {
        ui->disassemblyDock->hide();
        ui->memoryDock->hide();
        ui->registersDock->hide();
        ui->stackDock->hide();
    }
}

void Chip8MainWindow::update_debug_widgets()
{
    chip8_disassembly_->update_disassembly(chip8_.get_memory(),
                                           chip8_.get_pc());

    chip8_memory_->update_memory(chip8_.get_memory());

    chip8_registers_->update_registers(chip8_.get_v(),
                                       chip8_.get_i(),
                                       chip8_.get_pc(),
                                       chip8_.get_sp(),
                                       chip8_.get_delay_timer(),
                                       chip8_.get_sound_timer());

    chip8_stack_->update_stack(chip8_.get_stack(),
                               chip8_.get_sp());
}
