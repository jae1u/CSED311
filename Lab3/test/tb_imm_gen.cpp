#include "Simulator.h"
#include "Vimm_gen.h"

SIM_COMBI(imm_gen, {
    SIM_SETUP({
        m.instr = 0;
    })

    SIM_CASE("R-Type", {
        m.instr = 0b0000000'00001'00001'000'00001'01100'11U; // add x1, x1, x1
        s.step();
        SIM_CHECK_PORT(imm, 0);
    })

    SIM_CASE("I-Type", {
        m.instr = 0b010011100111'00000'111'00000'00100'11U; // addi x0, x0, <imm>
        s.step();
        SIM_CHECK_PORT(imm, 0b010011100111);
    })

    SIM_CASE("I-Type shift", {
        m.instr = 0b0100000'10010'00000'001'00000'00100'11U; // srai x0, x0, 0b10010
        s.step();
        SIM_CHECK_PORT(imm, 0b10010);
    })

    SIM_CASE("J-Type", {
        m.instr = 0b1'1100011001'0'10011001'00000'11011'11U; // jar x0, 0b1'1001100'0'11000110011'0
        s.step();
        SIM_CHECK_PORT(imm, 0b11111111111'1'10011001'0'1100011001'0);
    })

    SIM_CASE("B-Type", {
        m.instr = 0b1'100101'00000'00000'000'1111'0'11000'11U; // beq x0, x0, 0b1'0'100101'1111'0
        s.step();
        SIM_CHECK_PORT(imm, 0b1111111111111111111'1'0'100101'1111'0U);
    })

    SIM_CASE("S-Type", {
        m.instr = 0b1100110'00000'00000'010'00110'01000'11U; // sw x0, 0b1100110'00110(x0)
        s.step();
        SIM_CHECK_PORT(imm, 0b11111111111111111111'1100110'00110);
    })

    SIM_CASE("JALR", {
        m.instr = 0b011001011001'00000'000'00000'11001'11U; // jalr x0, 0b011001011001(x0)
        s.step();
        SIM_CHECK_PORT(imm, 0b011001011001);
    })
  
    SIM_CASE("LW", {
        m.instr = 0b00000000100000010010000010000011;   // lw x1, 8(x2)
        s.step();
        SIM_CHECK_PORT(imm, 8);
    })
})