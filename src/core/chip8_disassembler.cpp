#include "core/chip8_disassembler.hpp"

#include <iomanip>
#include <sstream>

namespace chip8 {
namespace {

/**
 * @brief Formats a value as a zero-padded, lowercase hexadecimal literal.
 * @param value Numeric value to format.
 * @param width Minimum number of hexadecimal digits.
 * @return String of the form "0x0ab".
 */
std::string to_hex(unsigned int value, int width) {
    std::ostringstream stream;
    stream << "0x" << std::hex << std::setw(width) << std::setfill('0') << value;
    return stream.str();
}

/**
 * @brief Returns the register mnemonic for a nibble, e.g. "V3".
 * @param reg Register index in the range [0, 15].
 * @return Register name.
 */
std::string reg_name(std::uint8_t reg) {
    std::ostringstream stream;
    stream << 'V' << std::hex << static_cast<int>(reg);
    return stream.str();
}

} // namespace

std::string Disassembler::disassemble_opcode(std::uint16_t opcode) {
    const std::uint8_t x = (opcode & 0x0F00) >> 8;
    const std::uint8_t y = (opcode & 0x00F0) >> 4;
    const std::uint8_t n = opcode & 0x000F;
    const std::uint8_t nn = opcode & 0x00FF;
    const std::uint16_t nnn = opcode & 0x0FFF;

    const std::string unknown = "UNKNOWN " + to_hex(opcode, 4);

    switch (opcode & 0xF000) {
    case 0x0000:
        switch (opcode) {
        case 0x00E0:
            return "CLS";
        case 0x00EE:
            return "RET";
        default:
            return "SYS " + to_hex(nnn, 3);
        }

    case 0x1000:
        return "JP " + to_hex(nnn, 3);

    case 0x2000:
        return "CALL " + to_hex(nnn, 3);

    case 0x3000:
        return "SE " + reg_name(x) + ", " + to_hex(nn, 2);

    case 0x4000:
        return "SNE " + reg_name(x) + ", " + to_hex(nn, 2);

    case 0x5000:
        return "SE " + reg_name(x) + ", " + reg_name(y);

    case 0x6000:
        return "LD " + reg_name(x) + ", " + to_hex(nn, 2);

    case 0x7000:
        return "ADD " + reg_name(x) + ", " + to_hex(nn, 2);

    case 0x8000:
        switch (n) {
        case 0x0:
            return "LD " + reg_name(x) + ", " + reg_name(y);
        case 0x1:
            return "OR " + reg_name(x) + ", " + reg_name(y);
        case 0x2:
            return "AND " + reg_name(x) + ", " + reg_name(y);
        case 0x3:
            return "XOR " + reg_name(x) + ", " + reg_name(y);
        case 0x4:
            return "ADD " + reg_name(x) + ", " + reg_name(y);
        case 0x5:
            return "SUB " + reg_name(x) + ", " + reg_name(y);
        case 0x6:
            return "SHR " + reg_name(x);
        case 0x7:
            return "SUBN " + reg_name(x) + ", " + reg_name(y);
        case 0xE:
            return "SHL " + reg_name(x);
        default:
            return unknown;
        }

    case 0x9000:
        return "SNE " + reg_name(x) + ", " + reg_name(y);

    case 0xA000:
        return "LD I, " + to_hex(nnn, 3);

    case 0xB000:
        return "JP V0, " + to_hex(nnn, 3);

    case 0xC000:
        return "RND " + reg_name(x) + ", " + to_hex(nn, 2);

    case 0xD000:
        return "DRW " + reg_name(x) + ", " + reg_name(y) + ", " +
               std::to_string(static_cast<int>(n));

    case 0xE000:
        switch (nn) {
        case 0x9E:
            return "SKP " + reg_name(x);
        case 0xA1:
            return "SKNP " + reg_name(x);
        default:
            return unknown;
        }

    case 0xF000:
        switch (nn) {
        case 0x07:
            return "LD " + reg_name(x) + ", DT";
        case 0x0A:
            return "LD " + reg_name(x) + ", K";
        case 0x15:
            return "LD DT, " + reg_name(x);
        case 0x18:
            return "LD ST, " + reg_name(x);
        case 0x1E:
            return "ADD I, " + reg_name(x);
        case 0x29:
            return "LD F, " + reg_name(x);
        case 0x33:
            return "LD B, " + reg_name(x);
        case 0x55:
            return "LD [I], " + reg_name(x);
        case 0x65:
            return "LD " + reg_name(x) + ", [I]";
        default:
            return unknown;
        }

    default:
        return unknown;
    }
}

std::vector<DisassemblyLine> Disassembler::disassemble_memory(
    const std::array<std::uint8_t, 4096>& memory,
    std::uint16_t start_addr,
    std::uint16_t end_addr) {
    std::vector<DisassemblyLine> result;

    if (start_addr >= memory.size() || end_addr > memory.size() ||
        start_addr >= end_addr) {
        return result;
    }

    for (std::uint16_t addr = start_addr; addr + 1 < end_addr; addr += 2) {
        const std::uint16_t opcode =
            static_cast<std::uint16_t>(memory[addr] << 8) | memory[addr + 1];
        result.emplace_back(addr, disassemble_opcode(opcode));
    }

    return result;
}

} // namespace chip8
