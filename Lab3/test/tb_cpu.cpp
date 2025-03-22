#include <vector>
#include "Simulator.h"
#include "Vcpu.h"
#include "Vcpu_cpu.h"
#include "Vcpu_register_file.h"
#include "Vcpu_memory.h"
#include "helpers.h"

constexpr int MAX_TICK = 1000;


SIM_SEQ(cpu, {    
    SIM_SEQ_SETUP({
        m.cpu->memory_mod->_testing_manual_reset = 1;
        reset_memory<Vcpu_memory>(m.cpu->memory_mod);
        m.cpu->rf->_testing_manual_reset = 1;
        reset_rf<Vcpu_register_file>(m.cpu->rf);
    })
    
    SIM_SEQ_CASE("addi", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 5}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02a10093}, 0); // addi x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 47);
    })

    SIM_SEQ_CASE("slti less", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, -42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02a12093}, 0); // slti x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 1);
    })

    SIM_SEQ_CASE("slti equal", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02a12093}, 0); // slti x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0);
    })

    SIM_SEQ_CASE("slti more", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 84}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02a12093}, 0); // slti x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0);
    })

    SIM_SEQ_CASE("sltiu less", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 24}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02a13093}, 0); // sltiu x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 1);
    })

    SIM_SEQ_CASE("sltiu equal", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02a13093}, 0); // sltiu x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0);
    })

    SIM_SEQ_CASE("sltiu more", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 84}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02a13093}, 0); // sltiu x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0);
    })

    SIM_SEQ_CASE("andi", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 0b000111}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02a17093}, 0); // andi x1, x2, 42 (=0b101010)
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0b000010);
    })

    SIM_SEQ_CASE("ori", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 0b000111}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02a16093}, 0); // ori x1, x2, 42 (=0b101010)
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0b101111);
    })

    SIM_SEQ_CASE("xori", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 0b000111}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02a14093}, 0); // xori x1, x2, 42 (=0b101010)
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0b101101);
    })

    SIM_SEQ_CASE("slli", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x00311093}, 0); // slli x1, x2, 3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 42 * 8);
    })

    SIM_SEQ_CASE("srli", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x00215093}, 0); // srli x1, x2, 2
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 10);
    })

    SIM_SEQ_CASE("srai", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, -42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x40215093}, 0); // srai x1, x2, 2
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], -11);
    })

    SIM_SEQ_CASE("add", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 1},
            {3, 2}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x003100b3}, 0); // add x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 3);
    })

    SIM_SEQ_CASE("slt less", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, -1},
            {3, 2}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x003120b3}, 0); // slt x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 1);
    })

    SIM_SEQ_CASE("slt equal", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 2},
            {3, 2}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x003120b3}, 0); // slt x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0);
    })

    SIM_SEQ_CASE("slt more", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 42},
            {3, 2}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x003120b3}, 0); // slt x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0);
    })

    SIM_SEQ_CASE("sltu less", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 2},
            {3, 5}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x003130b3}, 0); // sltu x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 1);
    })

    SIM_SEQ_CASE("sltu equal", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 5},
            {3, 5}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x003130b3}, 0); // sltu x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0);
    })

    SIM_SEQ_CASE("sltu more", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 7},
            {3, 5}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x003130b3}, 0); // sltu x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0);
    })

    SIM_SEQ_CASE("and", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 0b000111},
            {3, 0b101010}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x003170b3}, 0); // and x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0b000010);
    })

    SIM_SEQ_CASE("or", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 0b000111},
            {3, 0b101010}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x003160b3}, 0); // or x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0b101111);
    })

    SIM_SEQ_CASE("xor", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 0b000111},
            {3, 0b101010}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x003140b3}, 0); // xor x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 0b101101);
    })

    SIM_SEQ_CASE("sll", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 42},
            {3, 3}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x003110b3}, 0); // sll x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 42 * 8);
    })

    SIM_SEQ_CASE("srl", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 42},
            {3, 2}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x003150b3}, 0); // srl x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], 10);
    })

    SIM_SEQ_CASE("sra", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, -42},
            {3, 2}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x403150b3}, 0); // sra x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], -11);
    })

    SIM_SEQ_CASE("sub", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 24},
            {3, 48}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x403100b3}, 0); // sub x1, x2, x3
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK_PORT(reg_data[1], -24);
    })

    SIM_SEQ_CASE("jal", {
        m.reset = 1;
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02a000ef}, 0); // jal x1, 0x2a
        s.tick();
        m.reset = 0;
        s.tick(3);
        SIM_CHECK(m.cpu->pc_val, 0x2a, "pc");
        SIM_CHECK_PORT(reg_data[1], 4);
    })

    SIM_SEQ_CASE("jalr", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 0x30},
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02a100e7}, 0); // jalr x1, 0x2a(x2)
        s.tick();
        m.reset = 0;
        s.tick(3);
        SIM_CHECK(m.cpu->pc_val, 0x5a, "pc");
        SIM_CHECK_PORT(reg_data[1], 4);
    })

    SIM_SEQ_CASE("bne bcond=0", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {1, 42},
            {2, 42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02209563}, 0); // bne x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK(m.cpu->pc_val, 4, "pc");
    })

    SIM_SEQ_CASE("bne bcond=1", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {1, 24},
            {2, 42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02209563}, 0); // bne x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK(m.cpu->pc_val, 42, "pc");
    })

    SIM_SEQ_CASE("beq bcond=0", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {1, 42},
            {2, 24}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02208563}, 0); // beq x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK(m.cpu->pc_val, 4, "pc");
    })

    SIM_SEQ_CASE("beq bcond=1", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {1, 42},
            {2, 42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x02208563}, 0); // bne x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK(m.cpu->pc_val, 42, "pc");
    })

    SIM_SEQ_CASE("blt bcond=0", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {1, 42},
            {2, -24}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x0220c563}, 0); // blt x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK(m.cpu->pc_val, 4, "pc");
    })

    SIM_SEQ_CASE("blt bcond=1", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {1, -24},
            {2, 42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x0220c563}, 0); // blt x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK(m.cpu->pc_val, 42, "pc");
    })

    SIM_SEQ_CASE("bltu bcond=0", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {1, 42},
            {2, 24}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x0220e563}, 0); // bltu x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK(m.cpu->pc_val, 4, "pc");
    })

    SIM_SEQ_CASE("bltu bcond=1", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {1, 24},
            {2, 42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x0220e563}, 0); // bltu x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK(m.cpu->pc_val, 42, "pc");
    })

    SIM_SEQ_CASE("bge bcond=0", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {1, -24},
            {2, 42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x0220d563}, 0); // bge x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK(m.cpu->pc_val, 4, "pc");
    })

    SIM_SEQ_CASE("bge bcond=1", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {1, 24},
            {2, -42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x0220d563}, 0); // bge x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK(m.cpu->pc_val, 42, "pc");
    })

    SIM_SEQ_CASE("bgeu bcond=0", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {1, 24},
            {2, 42}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x0220f563}, 0); // bgeu x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK(m.cpu->pc_val, 4, "pc");
    })

    SIM_SEQ_CASE("bgeu bcond=1", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {1, 42},
            {2, 24}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x0220f563}, 0); // bgeu x1, x2, 42
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK(m.cpu->pc_val, 42, "pc");
    })

    SIM_SEQ_CASE("lw", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {2, 92}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x00812083}, 0); // lw x1, 8(x2)
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0xdeadbeef}, 100);
        s.tick();
        m.reset = 0;
        s.tick(5);
        SIM_CHECK_PORT(reg_data[1], 0xdeadbeef);
    })

    SIM_SEQ_CASE("sw", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {1, 0xdeadbeef},
            {2, 92}
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x00112423}, 0); // sw x1, 8(x2)
        s.tick();
        m.reset = 0;
        s.tick(4);
        SIM_CHECK(m.cpu->memory_mod->mem[25], 0xdeadbeef, "mem@100");
    })

    SIM_SEQ_CASE("ecall", {
        m.reset = 1;
        init_rf<Vcpu_register_file>(m.cpu->rf, {
            {17, 10},
        });
        init_memory<Vcpu_memory>(m.cpu->memory_mod, {0x00000073}, 0); // ecall
        s.tick();
        m.reset = 0;
        s.tick(2);
        SIM_CHECK_PORT(is_halted, 1);
    })

    SIM_SEQ_CASE("non_controlflow", {
        m.reset = 1;
        load_instr_to_memory<Vcpu_memory>(m.cpu->memory_mod, "test/test_programs/non_controlflow.txt");
        s.tick();
        m.reset = 0;

        int i = 0;
        while (i < MAX_TICK && !m.is_halted) {
            s.tick();
            ++i;
        }
        SIM_CHECK(m.is_halted, 1, "is_halted");

        auto expected_reg_data = load_hex_dump("test/test_programs/non_controlflow.reg_state");
        for (int i = 1; i < 32; ++i) {
            SIM_CHECK(m.reg_data[i], expected_reg_data[i], std::string("x") + std::to_string(i));
        }
    })

    SIM_SEQ_CASE("basic_mem", {
        m.reset = 1;
        load_instr_to_memory<Vcpu_memory>(m.cpu->memory_mod, "test/test_programs/basic_mem.txt");
        s.tick();
        m.reset = 0;

        int i = 0;
        while (i < MAX_TICK && !m.is_halted) {
            s.tick();
            ++i;
        }
        SIM_CHECK(m.is_halted, 1, "is_halted");

        auto expected_reg_data = load_hex_dump("test/test_programs/basic_mem.reg_state");
        for (int i = 1; i < 32; ++i) {
            SIM_CHECK(m.reg_data[i], expected_reg_data[i], std::string("x") + std::to_string(i));
        }
    })

    SIM_SEQ_CASE("loop_mem", {
        m.reset = 1;
        load_instr_to_memory<Vcpu_memory>(m.cpu->memory_mod, "test/test_programs/loop_mem.txt");
        s.tick();
        m.reset = 0;

        int i = 0;
        while (i < MAX_TICK && !m.is_halted) {
            s.tick();
            ++i;
        }
        SIM_CHECK(m.is_halted, 1, "is_halted");

        auto expected_reg_data = load_hex_dump("test/test_programs/loop_mem.reg_state");
        for (int i = 1; i < 32; ++i) {
            SIM_CHECK(m.reg_data[i], expected_reg_data[i], std::string("x") + std::to_string(i));
        }
    })
})