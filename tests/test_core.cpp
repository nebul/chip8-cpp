#include "core/chip8.hpp"
#include "core/chip8_disassembler.hpp"

#include <cstdint>
#include <iostream>
#include <vector>

namespace {

int g_failures = 0;

void check(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++g_failures;
    }
}

std::vector<std::uint8_t> bytes(std::initializer_list<std::uint16_t> opcodes) {
    std::vector<std::uint8_t> program;
    for (std::uint16_t opcode : opcodes) {
        program.push_back(static_cast<std::uint8_t>(opcode >> 8));
        program.push_back(static_cast<std::uint8_t>(opcode & 0xFF));
    }
    return program;
}

void test_load_and_immediate_ops() {
    chip8::Chip8 cpu;
    cpu.load_program(bytes({0x600A, 0x7005}));

    cpu.run_cycle();
    check(cpu.registers()[0] == 0x0A, "LD V0, 0x0A sets V0 to 10");

    cpu.run_cycle();
    check(cpu.registers()[0] == 0x0F, "ADD V0, 0x05 yields 15");
    check(cpu.program_counter() == chip8::Chip8::PROGRAM_START_ADDRESS + 4,
          "program counter advances by two instructions");
}

void test_add_sets_carry() {
    chip8::Chip8 cpu;
    cpu.load_program(bytes({0x60FF, 0x6101, 0x8014}));
    cpu.run_cycle();
    cpu.run_cycle();
    cpu.run_cycle();
    check(cpu.registers()[0] == 0x00, "0xFF + 0x01 wraps to 0x00");
    check(cpu.registers()[0xF] == 1, "carry flag set on overflow");
}

void test_skip_if_equal() {
    chip8::Chip8 cpu;
    cpu.load_program(bytes({0x6005, 0x3005}));
    cpu.run_cycle();
    const std::uint16_t pc_before = cpu.program_counter();
    cpu.run_cycle();
    check(cpu.program_counter() == pc_before + 4, "SE skips next instruction when equal");
}

void test_jump() {
    chip8::Chip8 cpu;
    cpu.load_program(bytes({0x1234}));
    cpu.run_cycle();
    check(cpu.program_counter() == 0x234, "JP sets the program counter");
}

void test_call_and_return() {
    chip8::Chip8 cpu;
    cpu.load_program(bytes({0x2206}));
    cpu.run_cycle();
    check(cpu.program_counter() == 0x206, "CALL jumps to the target address");
    check(cpu.stack_pointer() == 1, "CALL pushes a stack frame");

    chip8::Chip8 returner;
    returner.load_program(bytes({0x2204, 0x0000, 0x00EE}));
    returner.run_cycle();
    returner.run_cycle();
    check(returner.program_counter() == chip8::Chip8::PROGRAM_START_ADDRESS + 2,
          "RET resumes after the CALL instruction");
}

void test_draw_sets_collision_flag() {
    chip8::Chip8 cpu;
    cpu.load_program(bytes({0xA206, 0xD001, 0xD001, 0x8000}));
    cpu.run_cycle();
    cpu.run_cycle();
    check(cpu.registers()[0xF] == 0, "first draw reports no collision");
    check(cpu.display()[0] == 1, "top-left pixel is lit after draw");

    cpu.run_cycle();
    check(cpu.registers()[0xF] == 1, "redrawing the same sprite reports collision");
    check(cpu.display()[0] == 0, "overlapping pixel is cleared by XOR");
}

void test_timers_decouple_from_cpu() {
    chip8::Chip8 cpu;
    cpu.load_program(bytes({0x6005, 0xF015}));
    cpu.run_cycle();
    cpu.run_cycle();
    check(cpu.delay_timer() == 5, "LD DT, Vx loads the delay timer");

    cpu.run_cycle();
    check(cpu.delay_timer() == 5, "running cycles does not decrement timers");

    cpu.tick_timers();
    check(cpu.delay_timer() == 4, "tick_timers decrements the delay timer");
}

void test_sound_callback() {
    chip8::Chip8 cpu;
    bool beeping = false;
    cpu.set_sound_callback([&beeping](bool on) { beeping = on; });
    cpu.load_program(bytes({0x6001, 0xF018}));
    cpu.run_cycle();
    cpu.run_cycle();

    cpu.tick_timers();
    check(beeping, "sound callback fires when the sound timer starts");
    cpu.tick_timers();
    check(!beeping, "sound callback fires when the sound timer expires");
}

void test_disassembler() {
    check(chip8::Disassembler::disassemble_opcode(0x00E0) == "CLS", "disassemble CLS");
    check(chip8::Disassembler::disassemble_opcode(0x1234) == "JP 0x234", "disassemble JP");
    check(chip8::Disassembler::disassemble_opcode(0x600A) == "LD V0, 0x0a", "disassemble LD");
    check(chip8::Disassembler::disassemble_opcode(0xD125) == "DRW V1, V2, 5", "disassemble DRW");
    check(chip8::Disassembler::disassemble_opcode(0xFFFF) == "UNKNOWN 0xffff",
          "disassemble unknown opcode");

    chip8::Chip8 cpu;
    cpu.load_program(bytes({0x00E0, 0x1234}));
    const auto lines = chip8::Disassembler::disassemble_memory(
        cpu.memory(), chip8::Chip8::PROGRAM_START_ADDRESS,
        chip8::Chip8::PROGRAM_START_ADDRESS + 4);
    check(lines.size() == 2, "disassemble_memory returns one line per instruction");
    check(lines[0].second == "CLS", "first disassembled line is CLS");
}

} // namespace

int main() {
    test_load_and_immediate_ops();
    test_add_sets_carry();
    test_skip_if_equal();
    test_jump();
    test_call_and_return();
    test_draw_sets_collision_flag();
    test_timers_decouple_from_cpu();
    test_sound_callback();
    test_disassembler();

    if (g_failures == 0) {
        std::cout << "All core tests passed.\n";
        return 0;
    }
    std::cerr << g_failures << " core test(s) failed.\n";
    return 1;
}
