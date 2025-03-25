#include "Simulator.h"
#include "Valu.h"
#include "defs.h"

SIM_COMBI(alu, {
    SIM_TEARDOWN({
        m.a = 0;
        m.b = 0;
        m.alu_ctl = 0;
    })

    SIM_CASE("and", {
        m.a = 0b0011;
        m.b = 0b0101;
        m.alu_ctl = ALUCtl::AND;
        s.step();
        SIM_CHECK_PORT(result, 0b0001);
    })

    SIM_CASE("or", {
        m.a = 0b0011;
        m.b = 0b0101;
        m.alu_ctl = ALUCtl::OR;
        s.step();
        SIM_CHECK_PORT(result, 0b0111);
    })

    SIM_CASE("xor", {
        m.a = 0b0011;
        m.b = 0b0101;
        m.alu_ctl = ALUCtl::XOR;
        s.step();
        SIM_CHECK_PORT(result, 0b0110);
    })
    
    SIM_CASE("add", {
        m.a = 42;
        m.b = 24;
        m.alu_ctl = ALUCtl::ADD;
        s.step();
        SIM_CHECK_PORT(result, 66);
    })

    SIM_CASE("sub", {
        m.a = 21;
        m.b = 42;
        m.alu_ctl = ALUCtl::SUB;
        s.step();
        SIM_CHECK_PORT(result, -21);
    })

    SIM_CASE("shift left", {
        m.a = 0b1010;
        m.b = 2;
        m.alu_ctl = ALUCtl::SHIFT_LEFT;
        s.step();
        SIM_CHECK_PORT(result, 0b101000);
    })

    SIM_CASE("shift right logical", {
        m.a = 0b1100;
        m.b = 2;
        m.alu_ctl = ALUCtl::SHIFT_RIGHT_LOGICAL;
        s.step();
        SIM_CHECK_PORT(result, 0b11);
    })

    SIM_CASE("shift right arithmetic", {
        m.a = -16;
        m.b = 2;
        m.alu_ctl = ALUCtl::SHIFT_RIGHT_ARITHMETIC;
        s.step();
        SIM_CHECK_PORT(result, -4);
    })

    SIM_CASE("less than", {
        m.a = 21;
        m.b = -42;
        m.alu_ctl = ALUCtl::LESS_THAN;
        s.step();
        SIM_CHECK_PORT(result, 0);
    })

    SIM_CASE("greater equal", {
        m.a = 21;
        m.b = -42;
        m.alu_ctl = ALUCtl::GREATER_EQUAL;
        s.step();
        SIM_CHECK_PORT(result, 1);
    })

    SIM_CASE("less than unsigned", {
        m.a = 21;
        m.b = 42;
        m.alu_ctl = ALUCtl::LESS_THAN_U;
        s.step();
        SIM_CHECK_PORT(result, 1);
    })

    SIM_CASE("greater equal unsigned", {
        m.a = 21;
        m.b = 42;
        m.alu_ctl = ALUCtl::GREATER_EQUAL_U;
        s.step();
        SIM_CHECK_PORT(result, 0);
    })

    SIM_CASE("equal", {
        m.a = 21;
        m.b = 21;
        m.alu_ctl = ALUCtl::EQUAL;
        s.step();
        SIM_CHECK_PORT(result, 1);
    })

    SIM_CASE("not equal", {
        m.a = 21;
        m.b = 21;
        m.alu_ctl = ALUCtl::NOT_EQUAL;
        s.step();
        SIM_CHECK_PORT(result, 0);
    })
})