#include "Simulator.h"
#include "Valu_control.h"
#include "defs.h"

SIM_COMBI(alu_control, {
    SIM_TEARDOWN({
        m.alu_op = 0;
        m.funct = 0;
    })

    SIM_CASE("ADD", {
        m.alu_op = ALUOp::R_TYPE;
        m.funct = 0b0000;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::ADD);
    })

    SIM_CASE("SLT", {
        m.alu_op = ALUOp::R_TYPE;
        m.funct = 0b0010;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::LESS_THAN);
    })

    SIM_CASE("SLTU", {
        m.alu_op = ALUOp::R_TYPE;
        m.funct = 0b0011;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::LESS_THAN_U);
    })

    SIM_CASE("AND", {
        m.alu_op = ALUOp::R_TYPE;
        m.funct = 0b0111;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::AND);
    })

    SIM_CASE("OR", {
        m.alu_op = ALUOp::R_TYPE;
        m.funct = 0b0110;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::OR);
    })

    SIM_CASE("XOR", {
        m.alu_op = ALUOp::R_TYPE;
        m.funct = 0b0100;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::XOR);
    })

    SIM_CASE("SLL", {
        m.alu_op = ALUOp::R_TYPE;
        m.funct = 0b0001;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::SHIFT_LEFT);
    })

    SIM_CASE("SRL", {
        m.alu_op = ALUOp::R_TYPE;
        m.funct = 0b0101;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::SHIFT_RIGHT_LOGICAL);
    })

    SIM_CASE("SRA", {
        m.alu_op = ALUOp::R_TYPE;
        m.funct = 0b1101;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::SHIFT_RIGHT_ARITHMETIC);
    })

    SIM_CASE("SUB", {
        m.alu_op = ALUOp::R_TYPE;
        m.funct = 0b1000;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::SUB);
    })

    SIM_CASE("ADDI", {
        m.alu_op = ALUOp::I_TYPE;
        m.funct = 0b0000;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::ADD);
        
        m.funct = 0b1000;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::ADD);
    })

    SIM_CASE("SLTI", {
        m.alu_op = ALUOp::I_TYPE;
        m.funct = 0b0010;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::LESS_THAN);

        m.funct = 0b0010;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::LESS_THAN);
    })

    SIM_CASE("SLTUI", {
        m.alu_op = ALUOp::I_TYPE;
        m.funct = 0b0011;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::LESS_THAN_U);

        m.funct = 0b1011;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::LESS_THAN_U);
    })

    SIM_CASE("ANDI", {
        m.alu_op = ALUOp::I_TYPE;
        m.funct = 0b0111;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::AND);

        m.funct = 0b1111;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::AND);
    })

    SIM_CASE("ORI", {
        m.alu_op = ALUOp::I_TYPE;
        m.funct = 0b0110;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::OR);

        m.funct = 0b0110;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::OR);
    })

    SIM_CASE("XORI", {
        m.alu_op = ALUOp::I_TYPE;
        m.funct = 0b0100;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::XOR);

        m.funct = 0b1100;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::XOR);
    })

    SIM_CASE("SLLI", {
        m.alu_op = ALUOp::I_TYPE;
        m.funct = 0b0001;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::SHIFT_LEFT);

        m.funct = 0b1001;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::SHIFT_LEFT);
    })

    SIM_CASE("SRLI", {
        m.alu_op = ALUOp::I_TYPE;
        m.funct = 0b0101;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::SHIFT_RIGHT_LOGICAL);
    })

    SIM_CASE("SRAI", {
        m.alu_op = ALUOp::I_TYPE;
        m.funct = 0b1101;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::SHIFT_RIGHT_ARITHMETIC);
    })

    SIM_CASE("addition", {
        m.alu_op = ALUOp::ADD;
        for (unsigned i = 0; i < 0b10000; ++i) {
            m.funct = i;
            s.step();
            SIM_CHECK_PORT(alu_ctl, ALUCtl::ADD);
        }
    })

    SIM_CASE("BEQ", {
        m.alu_op = ALUOp::B;
        m.funct = 0b0000;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::EQUAL);

        m.funct = 0b1000;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::EQUAL);
    })

    SIM_CASE("BNE", {
        m.alu_op = ALUOp::B;
        m.funct = 0b0001;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::NOT_EQUAL);

        m.funct = 0b1001;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::NOT_EQUAL);
    })

    SIM_CASE("BLT", {
        m.alu_op = ALUOp::B;
        m.funct = 0b0100;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::LESS_THAN);

        m.funct = 0b1100;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::LESS_THAN);
    })

    SIM_CASE("BLTU", {
        m.alu_op = ALUOp::B;
        m.funct = 0b0110;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::LESS_THAN_U);

        m.funct = 0b1110;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::LESS_THAN_U);
    })

    SIM_CASE("BGE", {
        m.alu_op = ALUOp::B;
        m.funct = 0b0101;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::GREATER_EQUAL);

        m.funct = 0b1101;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::GREATER_EQUAL);
    })

    SIM_CASE("BGEU", {
        m.alu_op = ALUOp::B;
        m.funct = 0b0111;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::GREATER_EQUAL_U);

        m.funct = 0b1111;
        s.step();
        SIM_CHECK_PORT(alu_ctl, ALUCtl::GREATER_EQUAL_U);
    })
})