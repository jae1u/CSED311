#include "Simulator.h"
#include "Vucontroller.h"
#include "defs.h"

#define CHECK_CONTROLS_IF() SIM_CHECK_PORT(pc_write, 0); \
SIM_CHECK_PORT(pc_write_not_cond, 0); \
SIM_CHECK_PORT(i_or_d, 0); \
SIM_CHECK_PORT(mem_read, 1); \
SIM_CHECK_PORT(mem_write, 0); \
SIM_CHECK_PORT(ir_write, 1); \
SIM_CHECK_PORT(mem_to_reg, 0); \
SIM_CHECK_PORT(reg_write, 0); \
SIM_CHECK_PORT(alu_src_a, 1); \
SIM_CHECK_PORT(alu_src_b, 0b00); \
SIM_CHECK_PORT(alu_op, ALUOp::R_TYPE); \
SIM_CHECK_PORT(pc_src, 1); \
SIM_CHECK_PORT(alu_out_write, 0);

#define CHECK_CONTROLS_ID() SIM_CHECK_PORT(pc_write, 0); \
SIM_CHECK_PORT(pc_write_not_cond, 0); \
SIM_CHECK_PORT(i_or_d, 0); \
SIM_CHECK_PORT(mem_read, 0); \
SIM_CHECK_PORT(mem_write, 0); \
SIM_CHECK_PORT(ir_write, 0); \
SIM_CHECK_PORT(mem_to_reg, 0); \
SIM_CHECK_PORT(reg_write, 0); \
SIM_CHECK_PORT(alu_src_a, 0); \
SIM_CHECK_PORT(alu_src_b, 0b01); \
SIM_CHECK_PORT(alu_op, ALUOp::ADD); \
SIM_CHECK_PORT(pc_src, 1); \
SIM_CHECK_PORT(alu_out_write, 1);

#define CHECK_CONTROLS_REX() SIM_CHECK_PORT(pc_write, 0); \
SIM_CHECK_PORT(pc_write_not_cond, 0); \
SIM_CHECK_PORT(i_or_d, 0); \
SIM_CHECK_PORT(mem_read, 0); \
SIM_CHECK_PORT(mem_write, 0); \
SIM_CHECK_PORT(ir_write, 0); \
SIM_CHECK_PORT(mem_to_reg, 0); \
SIM_CHECK_PORT(reg_write, 0); \
SIM_CHECK_PORT(alu_src_a, 1); \
SIM_CHECK_PORT(alu_src_b, 0b00); \
SIM_CHECK_PORT(alu_op, ALUOp::R_TYPE); \
SIM_CHECK_PORT(pc_src, 1); \
SIM_CHECK_PORT(alu_out_write, 1);

#define CHECK_CONTROLS_RWB() SIM_CHECK_PORT(pc_write, 1); \
SIM_CHECK_PORT(pc_write_not_cond, 0); \
SIM_CHECK_PORT(i_or_d, 0); \
SIM_CHECK_PORT(mem_read, 0); \
SIM_CHECK_PORT(mem_write, 0); \
SIM_CHECK_PORT(ir_write, 0); \
SIM_CHECK_PORT(mem_to_reg, 0); \
SIM_CHECK_PORT(reg_write, 1); \
SIM_CHECK_PORT(alu_src_a, 0); \
SIM_CHECK_PORT(alu_src_b, 0b01); \
SIM_CHECK_PORT(alu_op, ALUOp::ADD); \
SIM_CHECK_PORT(pc_src, 0); \
SIM_CHECK_PORT(alu_out_write, 0); \

#define CHECK_CONTROLS_MEM_ADDR() SIM_CHECK_PORT(pc_write, 0); \
SIM_CHECK_PORT(pc_write_not_cond, 0); \
SIM_CHECK_PORT(i_or_d, 0); \
SIM_CHECK_PORT(mem_read, 0); \
SIM_CHECK_PORT(mem_write, 0); \
SIM_CHECK_PORT(ir_write, 0); \
SIM_CHECK_PORT(mem_to_reg, 0); \
SIM_CHECK_PORT(reg_write, 0); \
SIM_CHECK_PORT(alu_src_a, 1); \
SIM_CHECK_PORT(alu_src_b, 0b10); \
SIM_CHECK_PORT(alu_op, ALUOp::ADD); \
SIM_CHECK_PORT(pc_src, 1); \
SIM_CHECK_PORT(alu_out_write, 1);

#define CHECK_CONTROLS_LD_MEM_READ() SIM_CHECK_PORT(pc_write, 0); \
SIM_CHECK_PORT(pc_write_not_cond, 0); \
SIM_CHECK_PORT(i_or_d, 1); \
SIM_CHECK_PORT(mem_read, 1); \
SIM_CHECK_PORT(mem_write, 0); \
SIM_CHECK_PORT(ir_write, 0); \
SIM_CHECK_PORT(mem_to_reg, 0); \
SIM_CHECK_PORT(reg_write, 0); \
SIM_CHECK_PORT(alu_src_a, 1); \
SIM_CHECK_PORT(alu_src_b, 0b00); \
SIM_CHECK_PORT(alu_op, ALUOp::R_TYPE); \
SIM_CHECK_PORT(pc_src, 1); \
SIM_CHECK_PORT(alu_out_write, 0);

#define CHECK_CONTROLS_LD_WB() SIM_CHECK_PORT(pc_write, 1); \
SIM_CHECK_PORT(pc_write_not_cond, 0); \
SIM_CHECK_PORT(i_or_d, 0); \
SIM_CHECK_PORT(mem_read, 0); \
SIM_CHECK_PORT(mem_write, 0); \
SIM_CHECK_PORT(ir_write, 0); \
SIM_CHECK_PORT(mem_to_reg, 1); \
SIM_CHECK_PORT(reg_write, 1); \
SIM_CHECK_PORT(alu_src_a, 0); \
SIM_CHECK_PORT(alu_src_b, 0b01); \
SIM_CHECK_PORT(alu_op, ALUOp::ADD); \
SIM_CHECK_PORT(pc_src, 0); \
SIM_CHECK_PORT(alu_out_write, 0);

#define CHECK_CONTROLS_SD_MEM_WRITE() SIM_CHECK_PORT(pc_write, 1); \
SIM_CHECK_PORT(pc_write_not_cond, 0); \
SIM_CHECK_PORT(i_or_d, 1); \
SIM_CHECK_PORT(mem_read, 0); \
SIM_CHECK_PORT(mem_write, 1); \
SIM_CHECK_PORT(ir_write, 0); \
SIM_CHECK_PORT(mem_to_reg, 0); \
SIM_CHECK_PORT(reg_write, 0); \
SIM_CHECK_PORT(alu_src_a, 0); \
SIM_CHECK_PORT(alu_src_b, 0b01); \
SIM_CHECK_PORT(alu_op, ALUOp::ADD); \
SIM_CHECK_PORT(pc_src, 0); \
SIM_CHECK_PORT(alu_out_write, 0);

#define CHECK_CONTROLS_B_CHECK() SIM_CHECK_PORT(pc_write, 0); \
SIM_CHECK_PORT(pc_write_not_cond, 1); \
SIM_CHECK_PORT(i_or_d, 0); \
SIM_CHECK_PORT(mem_read, 0); \
SIM_CHECK_PORT(mem_write, 0); \
SIM_CHECK_PORT(ir_write, 0); \
SIM_CHECK_PORT(mem_to_reg, 0); \
SIM_CHECK_PORT(reg_write, 0); \
SIM_CHECK_PORT(alu_src_a, 1); \
SIM_CHECK_PORT(alu_src_b, 0b00); \
SIM_CHECK_PORT(alu_op, ALUOp::B); \
SIM_CHECK_PORT(pc_src, 1); \
SIM_CHECK_PORT(alu_out_write, 0);

#define CHECK_CONTROLS_BRANCH() SIM_CHECK_PORT(pc_write, 1); \
SIM_CHECK_PORT(pc_write_not_cond, 0); \
SIM_CHECK_PORT(i_or_d, 0); \
SIM_CHECK_PORT(mem_read, 0); \
SIM_CHECK_PORT(mem_write, 0); \
SIM_CHECK_PORT(ir_write, 0); \
SIM_CHECK_PORT(mem_to_reg, 0); \
SIM_CHECK_PORT(reg_write, 0); \
SIM_CHECK_PORT(alu_src_a, 0); \
SIM_CHECK_PORT(alu_src_b, 0b10); \
SIM_CHECK_PORT(alu_op, ALUOp::ADD); \
SIM_CHECK_PORT(pc_src, 0); \
SIM_CHECK_PORT(alu_out_write, 0);

#define CHECK_CONTROLS_JAL() SIM_CHECK_PORT(pc_write, 1); \
SIM_CHECK_PORT(pc_write_not_cond, 0); \
SIM_CHECK_PORT(i_or_d, 0); \
SIM_CHECK_PORT(mem_read, 0); \
SIM_CHECK_PORT(mem_write, 0); \
SIM_CHECK_PORT(ir_write, 0); \
SIM_CHECK_PORT(mem_to_reg, 0); \
SIM_CHECK_PORT(reg_write, 1); \
SIM_CHECK_PORT(alu_src_a, 0); \
SIM_CHECK_PORT(alu_src_b, 0b10); \
SIM_CHECK_PORT(alu_op, ALUOp::ADD); \
SIM_CHECK_PORT(pc_src, 0); \
SIM_CHECK_PORT(alu_out_write, 0);

#define CHECK_CONTROLS_JALR() SIM_CHECK_PORT(pc_write, 1); \
SIM_CHECK_PORT(pc_write_not_cond, 0); \
SIM_CHECK_PORT(i_or_d, 0); \
SIM_CHECK_PORT(mem_read, 0); \
SIM_CHECK_PORT(mem_write, 0); \
SIM_CHECK_PORT(ir_write, 0); \
SIM_CHECK_PORT(mem_to_reg, 0); \
SIM_CHECK_PORT(reg_write, 1); \
SIM_CHECK_PORT(alu_src_a, 1); \
SIM_CHECK_PORT(alu_src_b, 0b10); \
SIM_CHECK_PORT(alu_op, ALUOp::ADD); \
SIM_CHECK_PORT(pc_src, 0); \
SIM_CHECK_PORT(alu_out_write, 0);

#define CHECK_CONTROLS_IEX() SIM_CHECK_PORT(pc_write, 0); \
SIM_CHECK_PORT(pc_write_not_cond, 0); \
SIM_CHECK_PORT(i_or_d, 0); \
SIM_CHECK_PORT(mem_read, 0); \
SIM_CHECK_PORT(mem_write, 0); \
SIM_CHECK_PORT(ir_write, 0); \
SIM_CHECK_PORT(mem_to_reg, 0); \
SIM_CHECK_PORT(reg_write, 0); \
SIM_CHECK_PORT(alu_src_a, 1); \
SIM_CHECK_PORT(alu_src_b, 0b10); \
SIM_CHECK_PORT(alu_op, ALUOp::I_TYPE); \
SIM_CHECK_PORT(pc_src, 1); \
SIM_CHECK_PORT(alu_out_write, 1);

#define CHECK_CONTROLS_NOP() SIM_CHECK_PORT(pc_write, 1); \
SIM_CHECK_PORT(pc_write_not_cond, 0); \
SIM_CHECK_PORT(i_or_d, 0); \
SIM_CHECK_PORT(mem_read, 0); \
SIM_CHECK_PORT(mem_write, 0); \
SIM_CHECK_PORT(ir_write, 0); \
SIM_CHECK_PORT(mem_to_reg, 0); \
SIM_CHECK_PORT(reg_write, 0); \
SIM_CHECK_PORT(alu_src_a, 0); \
SIM_CHECK_PORT(alu_src_b, 0b01); \
SIM_CHECK_PORT(alu_op, ALUOp::ADD); \
SIM_CHECK_PORT(pc_src, 0); \
SIM_CHECK_PORT(alu_out_write, 0);


SIM_SEQ(ucontroller, {
    SIM_SEQ_SETUP({
        m.opcode = 0;
        m.bcond = 0;
        m.reset = 1;
        s.tick();
        m.reset = 0;
    })
    
    SIM_SEQ_CASE("R-Type", {
        m.opcode = 0b0110011;
        CHECK_CONTROLS_IF();
        s.tick();
        CHECK_CONTROLS_ID();
        s.tick();
        CHECK_CONTROLS_REX();
        s.tick();
        CHECK_CONTROLS_RWB();
        s.tick();
        CHECK_CONTROLS_IF();
    })

    SIM_SEQ_CASE("I-Type", {
        m.opcode = 0b0010011;
        CHECK_CONTROLS_IF();
        s.tick();
        CHECK_CONTROLS_ID();
        s.tick();
        CHECK_CONTROLS_IEX();
        s.tick();
        CHECK_CONTROLS_RWB();
        s.tick();
        CHECK_CONTROLS_IF();
    })

    SIM_SEQ_CASE("LOAD", {
        m.opcode = 0b0000011;
        CHECK_CONTROLS_IF();
        s.tick();
        CHECK_CONTROLS_ID();
        s.tick();
        CHECK_CONTROLS_MEM_ADDR();
        s.tick();
        CHECK_CONTROLS_LD_MEM_READ();
        s.tick();
        CHECK_CONTROLS_LD_WB();
        s.tick();
        CHECK_CONTROLS_IF();
    })

    SIM_SEQ_CASE("STORE", {
        m.opcode = 0b0100011;
        CHECK_CONTROLS_IF();
        s.tick();
        CHECK_CONTROLS_ID();
        s.tick();
        CHECK_CONTROLS_MEM_ADDR();
        s.tick();
        CHECK_CONTROLS_SD_MEM_WRITE();
        s.tick();
        CHECK_CONTROLS_IF();
    })

    SIM_SEQ_CASE("Bxx bcond=0", {
        m.opcode = 0b1100011;
        CHECK_CONTROLS_IF();
        s.tick();
        CHECK_CONTROLS_ID();
        s.tick();
        CHECK_CONTROLS_B_CHECK();
        m.bcond = 0;
        s.tick();
        CHECK_CONTROLS_IF();
    })

    SIM_SEQ_CASE("Bxx bcond=1", {
        m.opcode = 0b1100011;
        CHECK_CONTROLS_IF();
        s.tick();
        CHECK_CONTROLS_ID();
        s.tick();
        CHECK_CONTROLS_B_CHECK();
        m.bcond = 1;
        s.tick();
        CHECK_CONTROLS_BRANCH();
        s.tick();
        CHECK_CONTROLS_IF();
    })

    SIM_SEQ_CASE("JAL", {
        m.opcode = 0b1101111;
        CHECK_CONTROLS_IF();
        s.tick();
        CHECK_CONTROLS_ID();
        s.tick();
        CHECK_CONTROLS_JAL();
        s.tick();
        CHECK_CONTROLS_IF();
    })

    SIM_SEQ_CASE("JALR", {
        m.opcode = 0b1100111;
        CHECK_CONTROLS_IF();
        s.tick();
        CHECK_CONTROLS_ID();
        s.tick();
        CHECK_CONTROLS_JALR();
        s.tick();
        CHECK_CONTROLS_IF();
    })

    SIM_SEQ_CASE("ECALL", {
        m.opcode = 0b1110011;
        CHECK_CONTROLS_IF();
        s.tick();
        CHECK_CONTROLS_ID();
        s.tick();
        CHECK_CONTROLS_NOP();
        s.tick();
        CHECK_CONTROLS_IF();
    })
})