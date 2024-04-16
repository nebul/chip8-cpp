#include "core/chip8.hpp"

#include <algorithm>
#include <fstream>
#include <random>

namespace chip8 {
namespace {

/// Built-in 4x5 hexadecimal font set, loaded at the start of memory.
constexpr std::array<std::uint8_t, Chip8::FONTSET_SIZE> kFontset = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80};

/**
 * @brief Returns a random byte in the range [0, 255].
 *
 * The generator is seeded once on first use and shared across calls.
 */
std::uint8_t random_byte() {
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution<int> distribution(0, 255);
    return static_cast<std::uint8_t>(distribution(generator));
}

} // namespace

Chip8::Chip8() {
    reset();
}

void Chip8::reset() {
    memory_.fill(0);
    std::copy(kFontset.begin(), kFontset.end(), memory_.begin());

    v_.fill(0);
    stack_.fill(0);
    key_state_.fill(false);
    display_.fill(0);

    pc_ = PROGRAM_START_ADDRESS;
    i_ = 0;
    sp_ = 0;

    delay_timer_ = 0;
    sound_timer_ = 0;
    beep_state_ = false;

    notify_display();
}

void Chip8::run_cycle() {
    if (pc_ + 1 >= MEMORY_SIZE) {
        return;
    }

    const std::uint16_t opcode =
        static_cast<std::uint16_t>(memory_[pc_] << 8) | memory_[pc_ + 1];
    execute_opcode(opcode);
}

void Chip8::tick_timers() {
    if (delay_timer_ > 0) {
        --delay_timer_;
    }

    if (sound_timer_ > 0) {
        if (!beep_state_) {
            beep_state_ = true;
            if (sound_callback_) {
                sound_callback_(true);
            }
        }
        --sound_timer_;
    } else if (beep_state_) {
        beep_state_ = false;
        if (sound_callback_) {
            sound_callback_(false);
        }
    }
}

bool Chip8::load_program(const std::vector<std::uint8_t>& program) {
    if (program.size() > static_cast<std::size_t>(MEMORY_SIZE - PROGRAM_START_ADDRESS)) {
        return false;
    }

    std::copy(program.begin(), program.end(), memory_.begin() + PROGRAM_START_ADDRESS);
    return true;
}

bool Chip8::load_program_from_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        return false;
    }

    const std::streamsize size = file.tellg();
    if (size < 0 || size > MEMORY_SIZE - PROGRAM_START_ADDRESS) {
        return false;
    }

    file.seekg(0, std::ios::beg);
    std::vector<std::uint8_t> buffer(static_cast<std::size_t>(size));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return false;
    }

    return load_program(buffer);
}

void Chip8::set_key_state(std::uint8_t key, bool pressed) {
    if (key < KEY_COUNT) {
        key_state_[key] = pressed;
    }
}

void Chip8::set_cpu_frequency(std::uint32_t frequency) {
    cpu_frequency_ = frequency;
}

void Chip8::set_display_callback(DisplayCallback callback) {
    display_callback_ = std::move(callback);
}

void Chip8::set_sound_callback(SoundCallback callback) {
    sound_callback_ = std::move(callback);
}

bool Chip8::is_beeping() const {
    return beep_state_;
}

const std::array<std::uint8_t, Chip8::DISPLAY_WIDTH * Chip8::DISPLAY_HEIGHT>&
Chip8::display() const {
    return display_;
}

const std::array<std::uint8_t, Chip8::MEMORY_SIZE>& Chip8::memory() const {
    return memory_;
}

const std::array<std::uint8_t, Chip8::REGISTER_COUNT>& Chip8::registers() const {
    return v_;
}

const std::array<std::uint16_t, Chip8::STACK_SIZE>& Chip8::stack() const {
    return stack_;
}

std::uint16_t Chip8::program_counter() const {
    return pc_;
}

std::uint16_t Chip8::index_register() const {
    return i_;
}

std::uint8_t Chip8::stack_pointer() const {
    return sp_;
}

std::uint8_t Chip8::delay_timer() const {
    return delay_timer_;
}

std::uint8_t Chip8::sound_timer() const {
    return sound_timer_;
}

std::uint32_t Chip8::cpu_frequency() const {
    return cpu_frequency_;
}

void Chip8::execute_opcode(std::uint16_t opcode) {
    const std::uint8_t x = (opcode & 0x0F00) >> 8;
    const std::uint8_t y = (opcode & 0x00F0) >> 4;
    const std::uint8_t n = opcode & 0x000F;
    const std::uint8_t nn = opcode & 0x00FF;
    const std::uint16_t nnn = opcode & 0x0FFF;

    pc_ += 2;

    switch (opcode & 0xF000) {
    case 0x0000:
        switch (opcode & 0x00FF) {
        case 0x00E0:
            clear_display();
            break;
        case 0x00EE:
            if (sp_ > 0) {
                pc_ = stack_[--sp_];
            }
            break;
        default:
            break;
        }
        break;

    case 0x1000:
        pc_ = nnn;
        break;

    case 0x2000:
        if (sp_ < STACK_SIZE) {
            stack_[sp_++] = pc_;
            pc_ = nnn;
        }
        break;

    case 0x3000:
        if (v_[x] == nn) {
            pc_ += 2;
        }
        break;

    case 0x4000:
        if (v_[x] != nn) {
            pc_ += 2;
        }
        break;

    case 0x5000:
        if (v_[x] == v_[y]) {
            pc_ += 2;
        }
        break;

    case 0x6000:
        v_[x] = nn;
        break;

    case 0x7000:
        v_[x] += nn;
        break;

    case 0x8000:
        switch (n) {
        case 0x0:
            v_[x] = v_[y];
            break;
        case 0x1:
            v_[x] |= v_[y];
            break;
        case 0x2:
            v_[x] &= v_[y];
            break;
        case 0x3:
            v_[x] ^= v_[y];
            break;
        case 0x4: {
            const std::uint16_t sum = v_[x] + v_[y];
            v_[x] = sum & 0xFF;
            v_[0xF] = (sum > 0xFF) ? 1 : 0;
            break;
        }
        case 0x5: {
            const std::uint8_t borrow = (v_[x] >= v_[y]) ? 1 : 0;
            v_[x] -= v_[y];
            v_[0xF] = borrow;
            break;
        }
        case 0x6: {
            const std::uint8_t lsb = v_[x] & 0x1;
            v_[x] >>= 1;
            v_[0xF] = lsb;
            break;
        }
        case 0x7: {
            const std::uint8_t borrow = (v_[y] >= v_[x]) ? 1 : 0;
            v_[x] = v_[y] - v_[x];
            v_[0xF] = borrow;
            break;
        }
        case 0xE: {
            const std::uint8_t msb = (v_[x] & 0x80) >> 7;
            v_[x] <<= 1;
            v_[0xF] = msb;
            break;
        }
        default:
            break;
        }
        break;

    case 0x9000:
        if (v_[x] != v_[y]) {
            pc_ += 2;
        }
        break;

    case 0xA000:
        i_ = nnn;
        break;

    case 0xB000:
        pc_ = nnn + v_[0];
        break;

    case 0xC000:
        v_[x] = random_byte() & nn;
        break;

    case 0xD000:
        draw_sprite(x, y, n);
        break;

    case 0xE000:
        switch (nn) {
        case 0x9E:
            if (v_[x] < KEY_COUNT && key_state_[v_[x]]) {
                pc_ += 2;
            }
            break;
        case 0xA1:
            if (v_[x] < KEY_COUNT && !key_state_[v_[x]]) {
                pc_ += 2;
            }
            break;
        default:
            break;
        }
        break;

    case 0xF000:
        switch (nn) {
        case 0x07:
            v_[x] = delay_timer_;
            break;
        case 0x0A: {
            bool key_pressed = false;
            for (std::uint8_t key = 0; key < KEY_COUNT; ++key) {
                if (key_state_[key]) {
                    v_[x] = key;
                    key_pressed = true;
                    break;
                }
            }
            if (!key_pressed) {
                pc_ -= 2;
            }
            break;
        }
        case 0x15:
            delay_timer_ = v_[x];
            break;
        case 0x18:
            sound_timer_ = v_[x];
            break;
        case 0x1E:
            i_ += v_[x];
            break;
        case 0x29:
            i_ = v_[x] * 5;
            break;
        case 0x33:
            memory_[i_] = v_[x] / 100;
            memory_[i_ + 1] = (v_[x] / 10) % 10;
            memory_[i_ + 2] = v_[x] % 10;
            break;
        case 0x55:
            for (std::uint8_t reg = 0; reg <= x; ++reg) {
                memory_[i_ + reg] = v_[reg];
            }
            break;
        case 0x65:
            for (std::uint8_t reg = 0; reg <= x; ++reg) {
                v_[reg] = memory_[i_ + reg];
            }
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }
}

void Chip8::draw_sprite(std::uint8_t x, std::uint8_t y, std::uint8_t height) {
    const std::uint8_t origin_x = v_[x] % DISPLAY_WIDTH;
    const std::uint8_t origin_y = v_[y] % DISPLAY_HEIGHT;

    v_[0xF] = 0;

    for (std::uint8_t row = 0; row < height; ++row) {
        const std::uint8_t sprite_byte = memory_[i_ + row];
        for (std::uint8_t col = 0; col < 8; ++col) {
            if ((sprite_byte & (0x80 >> col)) == 0) {
                continue;
            }

            const std::uint8_t px = (origin_x + col) % DISPLAY_WIDTH;
            const std::uint8_t py = (origin_y + row) % DISPLAY_HEIGHT;
            const std::uint16_t index = py * DISPLAY_WIDTH + px;

            if (display_[index] == 1) {
                v_[0xF] = 1;
            }
            display_[index] ^= 1;
        }
    }

    notify_display();
}

void Chip8::clear_display() {
    display_.fill(0);
    notify_display();
}

void Chip8::notify_display() {
    if (display_callback_) {
        display_callback_();
    }
}

} // namespace chip8
