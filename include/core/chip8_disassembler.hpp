#ifndef CHIP8_CORE_CHIP8_DISASSEMBLER_HPP
#define CHIP8_CORE_CHIP8_DISASSEMBLER_HPP

#include <array>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace chip8 {

/// Pairs a memory address with the textual form of the instruction at it.
using DisassemblyLine = std::pair<std::uint16_t, std::string>;

/**
 * @brief Translates CHIP-8 opcodes into human-readable assembly mnemonics.
 *
 * The disassembler is stateless: every operation is exposed as a static helper
 * so it can be reused by debuggers, tooling or tests without instantiation.
 */
class Disassembler {
public:
    /**
     * @brief Disassembles a single opcode.
     * @param opcode 16-bit instruction word to decode.
     * @return Mnemonic representation, or an @c UNKNOWN entry for invalid
     *         opcodes.
     */
    static std::string disassemble_opcode(std::uint16_t opcode);

    /**
     * @brief Disassembles a contiguous range of memory.
     * @param memory Memory image to read from.
     * @param start_addr First address to decode (inclusive).
     * @param end_addr Address to stop at (exclusive).
     * @return One @ref DisassemblyLine per two-byte instruction in the range.
     *         An empty vector is returned when the range is invalid.
     */
    static std::vector<DisassemblyLine> disassemble_memory(
        const std::array<std::uint8_t, 4096>& memory,
        std::uint16_t start_addr,
        std::uint16_t end_addr);
};

} // namespace chip8

#endif // CHIP8_CORE_CHIP8_DISASSEMBLER_HPP
