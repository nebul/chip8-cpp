#include "ui/chip8_main_window.hpp"

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QTimer>

#include <algorithm>
#include <vector>

#include "ui/chip8_disassembly_widget.hpp"
#include "ui/chip8_display_widget.hpp"
#include "ui/chip8_memory_widget.hpp"
#include "ui/chip8_registers_widget.hpp"
#include "ui/chip8_stack_widget.hpp"

namespace chip8 {
namespace ui {
namespace {

/// Frame timer interval, in milliseconds, matching the 60 Hz timer rate.
constexpr int kFrameIntervalMs = 1000 / 60;

/**
 * @brief Maps a Qt key code to a CHIP-8 keypad index.
 * @param qt_key Key code reported by a Qt key event.
 * @return Keypad index in [0, 15], or 0xFF if the key is not mapped.
 */
std::uint8_t map_key(int qt_key) {
    switch (qt_key) {
    case Qt::Key_1: return 0x1;
    case Qt::Key_2: return 0x2;
    case Qt::Key_3: return 0x3;
    case Qt::Key_4: return 0xC;
    case Qt::Key_Q: return 0x4;
    case Qt::Key_W: return 0x5;
    case Qt::Key_E: return 0x6;
    case Qt::Key_R: return 0xD;
    case Qt::Key_A: return 0x7;
    case Qt::Key_S: return 0x8;
    case Qt::Key_D: return 0x9;
    case Qt::Key_F: return 0xE;
    case Qt::Key_Z: return 0xA;
    case Qt::Key_X: return 0x0;
    case Qt::Key_C: return 0xB;
    case Qt::Key_V: return 0xF;
    default: return 0xFF;
    }
}

} // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , display_(new DisplayWidget(this))
    , frame_timer_(new QTimer(this))
    , status_label_(new QLabel(this)) {
    setWindowTitle("CHIP-8 Emulator");
    setCentralWidget(display_);

    build_menu();
    build_debug_docks();

    statusBar()->addWidget(status_label_);
    update_status();

    frame_timer_->setInterval(kFrameIntervalMs);
    connect(frame_timer_, &QTimer::timeout, this, &MainWindow::run_frame);
    frame_timer_->start();

    refresh_views();
}

void MainWindow::build_menu() {
    QMenu* file_menu = menuBar()->addMenu("&File");
    connect(file_menu->addAction("&Open ROM..."), &QAction::triggered, this,
            &MainWindow::open_rom);
    file_menu->addSeparator();
    connect(file_menu->addAction("E&xit"), &QAction::triggered, this, &MainWindow::close);

    QMenu* emulation_menu = menuBar()->addMenu("&Emulation");
    connect(emulation_menu->addAction("&Pause/Resume"), &QAction::triggered, this,
            &MainWindow::toggle_pause);
    connect(emulation_menu->addAction("&Step"), &QAction::triggered, this,
            &MainWindow::step);
    connect(emulation_menu->addAction("&Reset"), &QAction::triggered, this,
            &MainWindow::reset);

    QMenu* view_menu = menuBar()->addMenu("&View");
    connect(view_menu->addAction("Toggle &Debug Views"), &QAction::triggered, this,
            &MainWindow::toggle_debug);
}

void MainWindow::build_debug_docks() {
    disassembly_ = new DisassemblyWidget(this);
    memory_ = new MemoryWidget(this);
    registers_ = new RegistersWidget(this);
    stack_ = new StackWidget(this);

    disassembly_dock_ = new QDockWidget("Disassembly", this);
    memory_dock_ = new QDockWidget("Memory", this);
    registers_dock_ = new QDockWidget("Registers", this);
    stack_dock_ = new QDockWidget("Stack", this);

    disassembly_dock_->setWidget(disassembly_);
    memory_dock_->setWidget(memory_);
    registers_dock_->setWidget(registers_);
    stack_dock_->setWidget(stack_);

    addDockWidget(Qt::LeftDockWidgetArea, disassembly_dock_);
    addDockWidget(Qt::RightDockWidgetArea, registers_dock_);
    addDockWidget(Qt::RightDockWidgetArea, stack_dock_);
    addDockWidget(Qt::BottomDockWidgetArea, memory_dock_);

    for (QDockWidget* dock :
         {disassembly_dock_, memory_dock_, registers_dock_, stack_dock_}) {
        dock->hide();
    }
}

void MainWindow::open_rom() {
    const QString path =
        QFileDialog::getOpenFileName(this, "Open ROM", QString(), "All Files (*.*)");
    if (path.isEmpty()) {
        return;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Unable to open the selected file.");
        return;
    }

    const QByteArray contents = file.readAll();
    const std::vector<std::uint8_t> program(contents.begin(), contents.end());

    core_.reset();
    if (!core_.load_program(program)) {
        QMessageBox::warning(this, "Error", "The ROM is too large to fit in memory.");
        return;
    }

    rom_loaded_ = true;
    running_ = true;
    update_status();
    refresh_views();
}

void MainWindow::toggle_pause() {
    if (!rom_loaded_) {
        return;
    }
    running_ = !running_;
    update_status();
}

void MainWindow::reset() {
    if (!rom_loaded_) {
        return;
    }
    core_.reset();
    refresh_views();
    update_status();
}

void MainWindow::step() {
    if (!rom_loaded_ || running_) {
        return;
    }
    core_.run_cycle();
    core_.tick_timers();
    refresh_views();
}

void MainWindow::toggle_debug() {
    debug_visible_ = !debug_visible_;
    for (QDockWidget* dock :
         {disassembly_dock_, memory_dock_, registers_dock_, stack_dock_}) {
        dock->setVisible(debug_visible_);
    }
    if (debug_visible_) {
        refresh_views();
    }
}

void MainWindow::run_frame() {
    if (!running_) {
        return;
    }

    const std::uint32_t cycles_per_frame = std::max<std::uint32_t>(1, core_.cpu_frequency() / 60);
    for (std::uint32_t i = 0; i < cycles_per_frame; ++i) {
        core_.run_cycle();
    }
    core_.tick_timers();

    if (core_.is_beeping()) {
        QApplication::beep();
    }

    refresh_views();
}

void MainWindow::refresh_views() {
    display_->update_display(core_.display());

    if (!debug_visible_) {
        return;
    }
    disassembly_->update_disassembly(core_);
    memory_->update_memory(core_);
    registers_->update_registers(core_);
    stack_->update_stack(core_);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (!event->isAutoRepeat()) {
        set_key_from_event(event, true);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    if (!event->isAutoRepeat()) {
        set_key_from_event(event, false);
    }
}

void MainWindow::set_key_from_event(QKeyEvent* event, bool pressed) {
    const std::uint8_t key = map_key(event->key());
    if (key != 0xFF) {
        core_.set_key_state(key, pressed);
    } else {
        event->ignore();
    }
}

void MainWindow::update_status() {
    QString state;
    if (!rom_loaded_) {
        state = "No ROM loaded";
    } else if (running_) {
        state = "Running";
    } else {
        state = "Paused";
    }
    status_label_->setText(
        QStringLiteral("%1  |  %2 Hz").arg(state).arg(core_.cpu_frequency()));
}

} // namespace ui
} // namespace chip8
