#ifndef CHIP8_CORE_CHIP8_HPP
#define CHIP8_CORE_CHIP8_HPP

#include <array>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace chip8 {

/**
 * @brief Cycle-accurate interpreter for the CHIP-8 virtual machine.
 *
 * The class models the complete CHIP-8 state: 4 KiB of memory, the general
 * purpose registers, the call stack, the delay and sound timers and the
 * monochrome 64x32 framebuffer. Instruction execution and timer updates are
 * decoupled so that the host can drive the CPU and the 60 Hz timers at
 * independent rates.
 *
 * @note The class performs no I/O of its own. Display refreshes and sound
 *       state changes are reported through optional callbacks, which keeps the
 *       core free of any UI or platform dependency.
 */
class Chip8 {
public:
    /// Total addressable memory in bytes.
    static constexpr std::uint16_t MEMORY_SIZE = 4096;
    /// Address at which user programs are loaded and start executing.
    static constexpr std::uint16_t PROGRAM_START_ADDRESS = 0x200;
    /// Framebuffer width in pixels.
    static constexpr std::uint8_t DISPLAY_WIDTH = 64;
    /// Framebuffer height in pixels.
    static constexpr std::uint8_t DISPLAY_HEIGHT = 32;
    /// Number of general purpose registers (V0..VF).
    static constexpr std::uint8_t REGISTER_COUNT = 16;
    /// Maximum call stack depth.
    static constexpr std::uint8_t STACK_SIZE = 16;
    /// Number of hexadecimal input keys.
    static constexpr std::uint8_t KEY_COUNT = 16;
    /// Size in bytes of the built-in hexadecimal font set.
    static constexpr std::uint8_t FONTSET_SIZE = 80;
    /// Default CPU frequency in instructions per second.
    static constexpr std::uint32_t DEFAULT_CPU_FREQUENCY = 700;

    /// Callback invoked whenever the framebuffer changes.
    using DisplayCallback = std::function<void()>;
    /// Callback invoked when the audio output is switched on or off.
    using SoundCallback = std::function<void(bool)>;

    /**
     * @brief Constructs an interpreter in a freshly initialized state.
     */
    Chip8();

    /**
     * @brief Restores the interpreter to its power-on state.
     *
     * Clears memory, registers, stack, timers and framebuffer, reloads the
     * built-in font set and points the program counter at
     * @ref PROGRAM_START_ADDRESS. Any previously loaded program is discarded.
     */
    void reset();

    /**
     * @brief Fetches, decodes and executes a single instruction.
     *
     * Timers are not affected; call @ref tick_timers separately at 60 Hz.
     */
    void run_cycle();

    /**
     * @brief Decrements the delay and sound timers by one step.
     *
     * Intended to be called by the host at 60 Hz, independently of the CPU
     * frequency. Triggers the sound callback when the audio output changes.
     */
    void tick_timers();

    /**
     * @brief Loads a program image into memory at @ref PROGRAM_START_ADDRESS.
     * @param program Raw program bytes.
     * @return @c true on success, @c false if the program is too large.
     */
    bool load_program(const std::vector<std::uint8_t>& program);

    /**
     * @brief Loads a program image from a binary file.
     * @param filename Path to the ROM file to read.
     * @return @c true on success, @c false if the file cannot be read or is
     *         too large to fit in memory.
     */
    bool load_program_from_file(const std::string& filename);

    /**
     * @brief Updates the pressed/released state of an input key.
     * @param key Key index in the range [0, @ref KEY_COUNT).
     * @param pressed @c true if the key is held down, @c false otherwise.
     */
    void set_key_state(std::uint8_t key, bool pressed);

    /**
     * @brief Sets the target CPU frequency.
     * @param frequency Number of instructions to execute per second.
     */
    void set_cpu_frequency(std::uint32_t frequency);

    /**
     * @brief Registers a callback fired whenever the framebuffer changes.
     * @param callback Function to invoke, or an empty target to disable.
     */
    void set_display_callback(DisplayCallback callback);

    /**
     * @brief Registers a callback fired when the audio output toggles.
     * @param callback Function receiving @c true when sound starts and
     *        @c false when it stops, or an empty target to disable.
     */
    void set_sound_callback(SoundCallback callback);

    /**
     * @brief Indicates whether the audio output is currently active.
     * @return @c true while a beep is playing.
     */
    [[nodiscard]] bool is_beeping() const;

    /**
     * @brief Provides read-only access to the framebuffer.
     * @return Row-major array of @ref DISPLAY_WIDTH x @ref DISPLAY_HEIGHT
     *         pixels, each either 0 (off) or 1 (on).
     */
    [[nodiscard]] const std::array<std::uint8_t, DISPLAY_WIDTH * DISPLAY_HEIGHT>&
    display() const;

    /**
     * @brief Provides read-only access to the full memory image.
     * @return Reference to the @ref MEMORY_SIZE byte memory array.
     */
    [[nodiscard]] const std::array<std::uint8_t, MEMORY_SIZE>& memory() const;

    /**
     * @brief Provides read-only access to the general purpose registers.
     * @return Reference to the V0..VF register array.
     */
    [[nodiscard]] const std::array<std::uint8_t, REGISTER_COUNT>& registers() const;

    /**
     * @brief Provides read-only access to the call stack.
     * @return Reference to the stack array.
     */
    [[nodiscard]] const std::array<std::uint16_t, STACK_SIZE>& stack() const;

    /**
     * @brief Returns the current program counter.
     * @return Address of the next instruction to execute.
     */
    [[nodiscard]] std::uint16_t program_counter() const;

    /**
     * @brief Returns the current index register value.
     * @return Value of the I register.
     */
    [[nodiscard]] std::uint16_t index_register() const;

    /**
     * @brief Returns the current stack pointer.
     * @return Number of frames currently on the call stack.
     */
    [[nodiscard]] std::uint8_t stack_pointer() const;

    /**
     * @brief Returns the current delay timer value.
     * @return Value of the delay timer.
     */
    [[nodiscard]] std::uint8_t delay_timer() const;

    /**
     * @brief Returns the current sound timer value.
     * @return Value of the sound timer.
     */
    [[nodiscard]] std::uint8_t sound_timer() const;

    /**
     * @brief Returns the configured CPU frequency.
     * @return Target instructions per second.
     */
    [[nodiscard]] std::uint32_t cpu_frequency() const;

private:
    /**
     * @brief Decodes and executes a single opcode.
     * @param opcode 16-bit instruction word to execute.
     */
    void execute_opcode(std::uint16_t opcode);

    /**
     * @brief Draws an n-byte sprite at (Vx, Vy) using XOR blending.
     * @param x Register index supplying the X coordinate.
     * @param y Register index supplying the Y coordinate.
     * @param height Number of sprite rows to draw.
     */
    void draw_sprite(std::uint8_t x, std::uint8_t y, std::uint8_t height);

    /**
     * @brief Clears the framebuffer and notifies the display callback.
     */
    void clear_display();

    /**
     * @brief Notifies the display callback if one is registered.
     */
    void notify_display();

    std::array<std::uint8_t, MEMORY_SIZE> memory_{};
    std::array<std::uint8_t, REGISTER_COUNT> v_{};
    std::array<std::uint16_t, STACK_SIZE> stack_{};
    std::array<std::uint8_t, DISPLAY_WIDTH * DISPLAY_HEIGHT> display_{};
    std::array<bool, KEY_COUNT> key_state_{};

    std::uint16_t pc_ = PROGRAM_START_ADDRESS;
    std::uint16_t i_ = 0;
    std::uint8_t sp_ = 0;

    std::uint8_t delay_timer_ = 0;
    std::uint8_t sound_timer_ = 0;
    bool beep_state_ = false;

    std::uint32_t cpu_frequency_ = DEFAULT_CPU_FREQUENCY;

    DisplayCallback display_callback_;
    SoundCallback sound_callback_;
};

} // namespace chip8

#endif // CHIP8_CORE_CHIP8_HPP
