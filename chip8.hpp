#include <array>
#include <cstdint>
#include <vector>

class Chip8
{
public:
    Chip8();
    ~Chip8();
    void initialize(void);
    void run_cycle(void);
    void load_program(const std::vector<uint8_t> &program);
    void set_key_state(uint8_t key, bool state);
    bool get_beep_state(void);
    std::array<uint8_t, 64U * 32U>& get_display_state(void);
    const std::array<uint8_t, 4096>& get_memory(void);
    uint16_t get_pc(void);
    const std::array<uint8_t, 16>& get_v(void);
    uint16_t get_i(void);
    uint8_t get_sp(void);
    uint8_t get_delay_timer(void);
    uint8_t get_sound_timer(void);
    const std::array<uint16_t, 16>& get_stack(void);

private:
    void execute_opcode(uint16_t opcode);
    void update_timers(void);
    void clear_display(void);

private:
    uint16_t pc_ = 0x200U;
    uint8_t sp_ = 0U;
    uint16_t i_ = 0U;
    std::array<uint8_t, 16U> v_{};
    std::array<uint8_t, 4096U> memory_{};
    std::array<uint16_t, 16U> stack_{};
    std::array<uint8_t, 64U * 32U> display_{};
    std::array<bool, 16U> key_state_{};
    bool beep_state_ = false;
    uint8_t delay_timer_ = 0;
    uint8_t sound_timer_ = 0;
};
