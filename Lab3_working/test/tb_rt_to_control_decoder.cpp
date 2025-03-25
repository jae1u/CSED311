#include "Simulator.h"
#include "defs.h"
#include "Vrt_to_control_decoder.h"

SIM_COMBI(rt_to_control_decoder, {
    SIM_SETUP({
        m.rt_encoding = 0;
    })

    SIM_CASE("rt #0", {
        m.rt_encoding = 1;
        s.step();
        SIM_CHECK_PORT(pc_write, 0);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 0);
        SIM_CHECK_PORT(mem_read, 1);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 1);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 1);
        SIM_CHECK_PORT(alu_src_b, 0b00);
        SIM_CHECK_PORT(alu_op, ALUOp::R_TYPE);
        SIM_CHECK_PORT(pc_src, 1);
        SIM_CHECK_PORT(alu_out_write, 0);
    })

    SIM_CASE("rt #1", {
        m.rt_encoding = 1 << 1;
        s.step();
        SIM_CHECK_PORT(pc_write, 0);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 0);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 1);
        SIM_CHECK_PORT(alu_src_b, 0b00);
        SIM_CHECK_PORT(alu_op, ALUOp::R_TYPE);
        SIM_CHECK_PORT(pc_src, 1);
        SIM_CHECK_PORT(alu_out_write, 0);
    })

    SIM_CASE("rt #2", {
        m.rt_encoding = 1 << 2;
        s.step();
        SIM_CHECK_PORT(pc_write, 0);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 0);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 1);
        SIM_CHECK_PORT(alu_src_b, 0b00);
        SIM_CHECK_PORT(alu_op, ALUOp::R_TYPE);
        SIM_CHECK_PORT(pc_src, 1);
        SIM_CHECK_PORT(alu_out_write, 0);
    })

    SIM_CASE("rt #3", {
        m.rt_encoding = 1 << 3;
        s.step();
        SIM_CHECK_PORT(pc_write, 0);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 0);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 0);
        SIM_CHECK_PORT(alu_src_b, 0b01);
        SIM_CHECK_PORT(alu_op, ALUOp::ADD);
        SIM_CHECK_PORT(pc_src, 1);
        SIM_CHECK_PORT(alu_out_write, 1);
    })

    SIM_CASE("rt #4", {
        m.rt_encoding = 1 << 4;
        s.step();
        SIM_CHECK_PORT(pc_write, 0);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 0);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 1);
        SIM_CHECK_PORT(alu_src_b, 0b00);
        SIM_CHECK_PORT(alu_op, ALUOp::R_TYPE);
        SIM_CHECK_PORT(pc_src, 1);
        SIM_CHECK_PORT(alu_out_write, 1);
    })

    SIM_CASE("rt #5", {
        m.rt_encoding = 1 << 5;
        s.step();
        SIM_CHECK_PORT(pc_write, 0);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 0);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 1);
        SIM_CHECK_PORT(alu_src_a, 1);
        SIM_CHECK_PORT(alu_src_b, 0b00);
        SIM_CHECK_PORT(alu_op, ALUOp::R_TYPE);
        SIM_CHECK_PORT(pc_src, 1);
        SIM_CHECK_PORT(alu_out_write, 0);
    })

    SIM_CASE("rt #6", {
        m.rt_encoding = 1 << 6;
        s.step();
        SIM_CHECK_PORT(pc_write, 1);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 0);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 0);
        SIM_CHECK_PORT(alu_src_b, 0b01);
        SIM_CHECK_PORT(alu_op, ALUOp::ADD);
        SIM_CHECK_PORT(pc_src, 0);
        SIM_CHECK_PORT(alu_out_write, 0);
    })

    SIM_CASE("rt #7", {
        m.rt_encoding = 1 << 7;
        s.step();
        SIM_CHECK_PORT(pc_write, 0);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 0);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 1);
        SIM_CHECK_PORT(alu_src_b, 0b10);
        SIM_CHECK_PORT(alu_op, ALUOp::ADD);
        SIM_CHECK_PORT(pc_src, 1);
        SIM_CHECK_PORT(alu_out_write, 1);
    })

    SIM_CASE("rt #8", {
        m.rt_encoding = 1 << 8;
        s.step();
        SIM_CHECK_PORT(pc_write, 0);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 1);
        SIM_CHECK_PORT(mem_read, 1);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 1);
        SIM_CHECK_PORT(alu_src_b, 0b00);
        SIM_CHECK_PORT(alu_op, ALUOp::R_TYPE);
        SIM_CHECK_PORT(pc_src, 1);
        SIM_CHECK_PORT(alu_out_write, 0);
    })

    SIM_CASE("rt #9", {
        m.rt_encoding = 1 << 9;
        s.step();
        SIM_CHECK_PORT(pc_write, 0);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 0);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 1);
        SIM_CHECK_PORT(reg_write, 1);
        SIM_CHECK_PORT(alu_src_a, 1);
        SIM_CHECK_PORT(alu_src_b, 0b00);
        SIM_CHECK_PORT(alu_op, ALUOp::R_TYPE);
        SIM_CHECK_PORT(pc_src, 1);
        SIM_CHECK_PORT(alu_out_write, 0);
    })

    SIM_CASE("rt #10", {
        m.rt_encoding = 1 << 10;
        s.step();
        SIM_CHECK_PORT(pc_write, 0);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 1);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 1);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 1);
        SIM_CHECK_PORT(alu_src_b, 0b00);
        SIM_CHECK_PORT(alu_op, ALUOp::R_TYPE);
        SIM_CHECK_PORT(pc_src, 1);
        SIM_CHECK_PORT(alu_out_write, 0);
    })

    SIM_CASE("rt #11", {
        m.rt_encoding = 1 << 11;
        s.step();
        SIM_CHECK_PORT(pc_write, 0);
        SIM_CHECK_PORT(pc_write_not_cond, 1);
        SIM_CHECK_PORT(i_or_d, 0);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 1);
        SIM_CHECK_PORT(alu_src_b, 0b00);
        SIM_CHECK_PORT(alu_op, ALUOp::B);
        SIM_CHECK_PORT(pc_src, 1);
        SIM_CHECK_PORT(alu_out_write, 0);
    })

    SIM_CASE("rt #12", {
        m.rt_encoding = 1 << 12;
        s.step();
        SIM_CHECK_PORT(pc_write, 1);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 0);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 0);
        SIM_CHECK_PORT(alu_src_b, 0b10);
        SIM_CHECK_PORT(alu_op, ALUOp::ADD);
        SIM_CHECK_PORT(pc_src, 0);
        SIM_CHECK_PORT(alu_out_write, 0);
    })

    SIM_CASE("rt #13", {
        m.rt_encoding = 1 << 13;
        s.step();
        SIM_CHECK_PORT(pc_write, 1);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 0);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 1);
        SIM_CHECK_PORT(alu_src_b, 0b10);
        SIM_CHECK_PORT(alu_op, ALUOp::ADD);
        SIM_CHECK_PORT(pc_src, 0);
        SIM_CHECK_PORT(alu_out_write, 0);
    })

    SIM_CASE("rt #14", {
        m.rt_encoding = 1 << 14;
        s.step();
        SIM_CHECK_PORT(pc_write, 0);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 0);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 0);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 1);
        SIM_CHECK_PORT(alu_src_b, 0b10);
        SIM_CHECK_PORT(alu_op, ALUOp::I_TYPE);
        SIM_CHECK_PORT(pc_src, 1);
        SIM_CHECK_PORT(alu_out_write, 1);
    })


    SIM_CASE("rt #6,10", {
        m.rt_encoding = (1 << 6) | (1 << 10);
        s.step();
        SIM_CHECK_PORT(pc_write, 1);
        SIM_CHECK_PORT(pc_write_not_cond, 0);
        SIM_CHECK_PORT(i_or_d, 1);
        SIM_CHECK_PORT(mem_read, 0);
        SIM_CHECK_PORT(mem_write, 1);
        SIM_CHECK_PORT(ir_write, 0);
        SIM_CHECK_PORT(mem_to_reg, 0);
        SIM_CHECK_PORT(reg_write, 0);
        SIM_CHECK_PORT(alu_src_a, 0);
        SIM_CHECK_PORT(alu_src_b, 0b01);
        SIM_CHECK_PORT(alu_op, ALUOp::ADD);
        SIM_CHECK_PORT(pc_src, 0);
        SIM_CHECK_PORT(alu_out_write, 0);
    })
})