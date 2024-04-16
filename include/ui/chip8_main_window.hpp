#ifndef CHIP8_UI_CHIP8_MAIN_WINDOW_HPP
#define CHIP8_UI_CHIP8_MAIN_WINDOW_HPP

#include <QMainWindow>

#include "core/chip8.hpp"

class QDockWidget;
class QLabel;
class QTimer;

namespace chip8 {
namespace ui {

class DisplayWidget;
class DisassemblyWidget;
class MemoryWidget;
class RegistersWidget;
class StackWidget;

/**
 * @brief Top-level window hosting the emulator and its debugging views.
 *
 * Owns the @ref chip8::Chip8 interpreter, drives it from a 60 Hz frame timer,
 * forwards keyboard input and exposes dockable register, memory, stack and
 * disassembly views.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Builds the window, its menu, central display and debug docks.
     * @param parent Owning widget, or @c nullptr.
     */
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    /**
     * @brief Maps a pressed key to the CHIP-8 keypad.
     * @param event Key event to handle.
     */
    void keyPressEvent(QKeyEvent* event) override;

    /**
     * @brief Maps a released key to the CHIP-8 keypad.
     * @param event Key event to handle.
     */
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    /**
     * @brief Prompts for a ROM file and loads it into the interpreter.
     */
    void open_rom();

    /**
     * @brief Toggles between the running and paused states.
     */
    void toggle_pause();

    /**
     * @brief Resets the interpreter and reloads the current ROM.
     */
    void reset();

    /**
     * @brief Executes a single instruction while paused.
     */
    void step();

    /**
     * @brief Shows or hides the debugging dock widgets.
     */
    void toggle_debug();

private:
    /**
     * @brief Creates the menu bar and its actions.
     */
    void build_menu();

    /**
     * @brief Creates the dockable debugging views.
     */
    void build_debug_docks();

    /**
     * @brief Runs one frame: a batch of CPU cycles plus a timer tick.
     */
    void run_frame();

    /**
     * @brief Refreshes the display and, when visible, the debug views.
     */
    void refresh_views();

    /**
     * @brief Updates the keypad state for the key carried by an event.
     * @param event Key event to translate.
     * @param pressed @c true on press, @c false on release.
     */
    void set_key_from_event(QKeyEvent* event, bool pressed);

    /**
     * @brief Updates the status bar text to reflect the current state.
     */
    void update_status();

    Chip8 core_;

    DisplayWidget* display_;
    DisassemblyWidget* disassembly_;
    MemoryWidget* memory_;
    RegistersWidget* registers_;
    StackWidget* stack_;

    QDockWidget* disassembly_dock_;
    QDockWidget* memory_dock_;
    QDockWidget* registers_dock_;
    QDockWidget* stack_dock_;

    QTimer* frame_timer_;
    QLabel* status_label_;

    bool rom_loaded_ = false;
    bool running_ = false;
    bool debug_visible_ = false;
};

} // namespace ui
} // namespace chip8

#endif // CHIP8_UI_CHIP8_MAIN_WINDOW_HPP
