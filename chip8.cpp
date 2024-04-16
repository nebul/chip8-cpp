#include <cstdlib>

#include "chip8.hpp"

Chip8::Chip8()
{
}

Chip8::~Chip8()
{
}

void Chip8::initialize()
{
    const std::array<uint8_t, 80U> chip8_fontset = {
        0xF0U, 0x90U, 0x90U, 0x90U, 0xF0U, // 0
        0x20U, 0x60U, 0x20U, 0x20U, 0x70U, // 1
        0xF0U, 0x10U, 0xF0U, 0x80U, 0xF0U, // 2
        0xF0U, 0x10U, 0xF0U, 0x10U, 0xF0U, // 3
        0x90U, 0x90U, 0xF0U, 0x10U, 0x10U, // 4
        0xF0U, 0x80U, 0xF0U, 0x10U, 0xF0U, // 5
        0xF0U, 0x80U, 0xF0U, 0x90U, 0xF0U, // 6
        0xF0U, 0x10U, 0x20U, 0x40U, 0x40U, // 7
        0xF0U, 0x90U, 0xF0U, 0x90U, 0xF0U, // 8
        0xF0U, 0x90U, 0xF0U, 0x10U, 0xF0U, // 9
        0xF0U, 0x90U, 0xF0U, 0x90U, 0x90U, // A
        0xE0U, 0x90U, 0xE0U, 0x90U, 0xE0U, // B
        0xF0U, 0x80U, 0x80U, 0x80U, 0xF0U, // C
        0xE0U, 0x90U, 0x90U, 0x90U, 0xE0U, // D
        0xF0U, 0x80U, 0xF0U, 0x80U, 0xF0U, // E
        0xF0U, 0x80U, 0xF0U, 0x80U, 0x80U  // F
    };

    memory_.fill(0U);
    std::copy(chip8_fontset.begin(),chip8_fontset.end(),memory_.begin());

    pc_ = 0x200U;
    i_ = 0U;
    sp_ = 0U;

    clear_display();

    stack_.fill(0U);
    v_.fill(0U);

    beep_state_ = false;
    delay_timer_ = 0U;
    sound_timer_ = 0U;

    key_state_.fill(false);
}

void Chip8::run_cycle()
{
    uint16_t opcode = memory_[pc_] << 8U | memory_[pc_ + 1U];
    execute_opcode(opcode);
    update_timers();
}

void Chip8::execute_opcode(uint16_t opcode)
{
    // Decode and execute opcode
    switch (opcode & 0xF000U)
    {
    case 0x0000U:
        switch (opcode & 0x00FFU)
        {
        case 0x00E0U:
            clear_display();
            pc_+=2U;
            break;
        case 0x00EEU:
            pc_ = stack_[sp_];
            sp_--;
            break;
        case 0x00F0U:
            break;
        default:
            break;
        }
        break;
    case 0x1000U:
        pc_ = opcode & 0x0FFFU;
        break;
    case 0x2000U:
        sp_++;
        stack_[sp_] = pc_;
        pc_ = opcode & 0x0FFFU;
        break;
    case 0x3000U:
        if (v_[((opcode & 0x0F00U) >> 8U)] == (opcode & 0x00FFU))
        {
            pc_ += 2U;
        }
        pc_ += 2U;
        break;
    case 0x4000U:
        if (v_[((opcode & 0x0F00U) >> 8U)] != (opcode & 0x00FFU))
        { 
            pc_ += 2U;
        }
        pc_ += 2U;
        break;
    case 0x5000U:
        if (v_[((opcode & 0x0F00U) >> 8U)] == v_[((opcode & 0x00F0U) >> 4U)])
        { 
            pc_ += 2U;
        }
        pc_ += 2U;
        break;
    case 0x6000U:
        v_[((opcode & 0x0F00U) >> 8U)] = opcode & 0x00FFU;
        pc_ += 2U;
        break;
    case 0x7000U:
        v_[((opcode & 0x0F00U) >> 8U)] += opcode & 0x00FFU;
        pc_ += 2U;
        break;
    case 0x8000U:
        switch (opcode & 0x000FU)
        {
        case 0x0000U:
            v_[((opcode & 0x0F00U) >> 8U)] = v_[((opcode & 0x00F0U) >> 4U)];
            pc_ += 2U;
            break;
        case 0x0001U:
            v_[((opcode & 0x0F00U) >> 8U)] |= v_[((opcode & 0x00F0U) >> 4U)];
            pc_ += 2U;
            break;
        case 0x0002U:
            v_[((opcode & 0x0F00U) >> 8U)] &= v_[((opcode & 0x00F0U) >> 4U)];
            pc_ += 2U;
            break;
        case 0x0003U:
            v_[((opcode & 0x0F00U) >> 8U)] ^= v_[((opcode & 0x00F0U) >> 4U)];
            pc_ += 2U;
            break;
        case 0x0004U:
            v_[0xFU] = ((v_[((opcode & 0x0F00U) >> 8U)] + v_[((opcode & 0x00F0U) >> 4U)]) > 0xFFU) ? 1U : 0U;
            v_[((opcode & 0x0F00U) >> 8U)] += v_[((opcode & 0x00F0U) >> 4U)];
            pc_ += 2U;
            break;
        case 0x0005U:
            v_[0xFU] = (v_[((opcode & 0x0F00U) >> 8U)] > v_[((opcode & 0x00F0U) >> 4U)]) ? 1U : 0U;
            v_[((opcode & 0x0F00U) >> 8U)] -= v_[((opcode & 0x00F0U) >> 4U)];
            pc_ += 2U;
            break;
        case 0x0006U:
            v_[0xFU] = v_[((opcode & 0x0F00U) >> 8U)] & 0x1U;
            v_[((opcode & 0x0F00U) >> 8U)] >>= 1U;
            pc_ += 2U;
            break;
        case 0x0007U:
            v_[0xFU] = (v_[((opcode & 0x00F0U) >> 4U)] > v_[((opcode & 0x0F00U) >> 8U)]) ? 1U : 0U;
            v_[((opcode & 0x0F00U) >> 8U)] = v_[((opcode & 0x00F0U) >> 4U)] - v_[((opcode & 0x0F00U) >> 8U)];
            pc_ += 2U;
            break;
        case 0x000EU:
            v_[0xFU] = (v_[((opcode & 0x0F00U) >> 8U)] & 0x80U) >> 7U;
            v_[((opcode & 0x0F00U) >> 8U)] <<= 1U;
            pc_ += 2U;
            break;
        default:
            break;
        }
        break;
    case 0x9000U:
        if (v_[((opcode & 0x0F00U) >> 8U)] != v_[((opcode & 0x00F0U) >> 4U)])
        { 
            pc_ += 2U;
        }
        pc_ += 2U;
        break;
    case 0xA000U:
        i_ = opcode & 0x0FFFU;
        pc_ += 2U;
        break;
    case 0xB000U:
        pc_ = (opcode & 0x0FFFU) + v_[0U];
        pc_ += 2U;
        break;
    case 0xC000U:
        v_[(opcode & 0x0F00U) >> 8U] = (rand() % 0xFFU) & (opcode & 0x00FFU);
        pc_ += 2U;
        break;
    case 0xD000U:
        for (uint8_t row = 0U; row < (opcode & 0x000FU); row++)
        {
            for (uint8_t column = 0U; column < 8U; column++)
            {
                if ((memory_[i_ + row] & (0x80U >> column)) != 0U)
                {
                    if (display_[(v_[(opcode & 0x0F00U) >> 8U] + column + ((v_[(opcode & 0x00F0) >> 4U] + row) * 64U))] == 1U)
                    {
                        v_[0xFU] = 1U;
                    }
                    display_[v_[(opcode & 0x0F00) >> 8U] + column + ((v_[(opcode & 0x00F0U) >> 4U] + row) * 64U)] ^= 1U;
                }
            }
        }
        pc_ += 2U;
        break;
    case 0xE000U:
        switch (opcode & 0x00FFU)
        {
        case 0x009EU:
            if (key_state_[v_[(opcode & 0x0F00U) >> 8U]])
            {
                pc_ += 2U;
            }
            pc_ += 2U;
            break;
        case 0x00A1U:
            if (!key_state_[v_[(opcode & 0x0F00U) >> 8U]])
            {
                pc_ += 2U;
            }
            pc_ += 2U;
            break;
        default:
            break;
        }
        break;
    case 0xF000U:
        switch (opcode & 0x00FFU)
        {
        case 0x0007U:
            v_[(opcode & 0x0F00U) >> 8U] = delay_timer_;
            pc_ += 2U;
            break;
        case 0x000AU:
            for (uint8_t index = 0U; index < 16U; index++)
            {
                if (key_state_[index])
                {
                    v_[(opcode & 0x0F00U) >> 8U] = index;
                }
            }
            pc_ += 2U;
            break;
        case 0x0015U:
            delay_timer_ = v_[(opcode & 0x0F00U) >> 8U];
            pc_ += 2U;
            break;
        case 0x0018U:
            sound_timer_ = v_[(opcode & 0x0F00U) >> 8U];
            pc_ += 2U;
            break;
        case 0x001EU:
            i_ += v_[(opcode & 0x0F00U) >> 8U];
            pc_ += 2U;
            break;
        case 0x0029U:
            i_ = v_[(opcode & 0x0F00U) >> 8U] * 0x5U;
            pc_ += 2U;
            break;
        case 0x0033U:
            memory_[i_] = v_[(opcode & 0x0F00U) >> 8U] / 100U;
            memory_[i_ + 1U] = (v_[(opcode & 0x0F00U) >> 8U] / 10U) % 10U;
            memory_[i_ + 2U] = v_[(opcode & 0x0F00U) >> 8U] % 10U;
            pc_ += 2U;
            break;
        case 0x0055U:
            for (uint16_t index = 0; index <= ((opcode & 0x0F00U) >> 8U); index++)
            {
                memory_[i_ + index] = v_[index];
            }
            pc_ += 2U;
            break;
        case 0x0065U:
            for (uint16_t index = 0; index <= ((opcode & 0x0F00U) >> 8U); index++)
            {
                v_[index] = memory_[i_ + index];
            }
            pc_ += 2U;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void Chip8::update_timers()
{
    if (delay_timer_ > 0U)
    {
        --delay_timer_;
    }

    if (sound_timer_ > 0U)
    {
        if (sound_timer_)
        {
            beep_state_ = true;
        }
        else
        {
            beep_state_ = false;
        }
        --sound_timer_;
    }
}

void Chip8::clear_display()
{
    display_.fill(0U);
}


void Chip8::load_program(const std::vector<uint8_t> &program)
{
    std::copy(program.begin(), program.end(), memory_.begin() + 0x200U);
}


void Chip8::set_key_state(uint8_t key, bool state)
{
    key_state_[key] = state;
}

std::array<uint8_t, 64U*32U>& Chip8::get_display_state(void)
{
    return display_;
}

bool Chip8::get_beep_state(void)
{
    return beep_state_;
}
