#include "Simulator.h"
#include "Vasl.h"

SIM_COMBI(asl, {
    SIM_SETUP({
        m.opcode = 0;
        m.bcond = 0;
        m.upc_incr = 0;
        m.seq_control = 0;
    })
    
    SIM_CASE("reset", {
        m.upc_incr = 1;
        m.seq_control = 0b000;
        s.step();
        SIM_CHECK_PORT(next_upc, 0);
    })

    SIM_CASE("dispatch ID opcode=RType", {
        m.seq_control = 0b001;
        m.opcode = 0b0110011;
        s.step();
        SIM_CHECK_PORT(next_upc, 2);
    })

    SIM_CASE("dispatch ID opcode=IType", {
        m.seq_control = 0b001;
        m.opcode = 0b0010011;
        s.step();
        SIM_CHECK_PORT(next_upc, 12);
    })

    SIM_CASE("dispatch ID opcode=LOAD", {
        m.seq_control = 0b001;
        m.opcode = 0b0000011;
        s.step();
        SIM_CHECK_PORT(next_upc, 4);
    })

    SIM_CASE("dispatch ID opcode=STORE", {
        m.seq_control = 0b001;
        m.opcode = 0b0100011;
        s.step();
        SIM_CHECK_PORT(next_upc, 4);
    })

    SIM_CASE("dispatch ID opcode=Bxx", {
        m.seq_control = 0b001;
        m.opcode = 0b1100011;
        s.step();
        SIM_CHECK_PORT(next_upc, 8);
    })

    SIM_CASE("dispatch ID opcode=JAL", {
        m.seq_control = 0b001;
        m.opcode = 0b1101111;
        s.step();
        SIM_CHECK_PORT(next_upc, 10);
    })

    SIM_CASE("dispatch ID opcode=JALR", {
        m.seq_control = 0b001;
        m.opcode = 0b1100111;
        s.step();
        SIM_CHECK_PORT(next_upc, 11);
    })

    SIM_CASE("dispatch ID opcode=ECALL", {
        m.seq_control = 0b001;
        m.opcode = 0b1110011;
        s.step();
        SIM_CHECK_PORT(next_upc, 13);
    })

    SIM_CASE("dispatch MemAddr opcode=LOAD", {
        m.seq_control = 0b010;
        m.opcode = 0b0000011;
        s.step();
        SIM_CHECK_PORT(next_upc, 5);
    })

    SIM_CASE("dispatch MemAddr opcode=STORE", {
        m.seq_control = 0b010;
        m.opcode = 0b0100011;
        s.step();
        SIM_CHECK_PORT(next_upc, 7);
    })

    SIM_CASE("dispatch BCheck bcond=0", {
        m.seq_control = 0b011;
        m.bcond = 0;
        s.step();
        SIM_CHECK_PORT(next_upc, 0);
    })

    SIM_CASE("dispatch BCheck bcond=1", {
        m.seq_control = 0b011;
        m.bcond = 1;
        s.step();
        SIM_CHECK_PORT(next_upc, 9);
    })

    SIM_CASE("dispatch IEX", {
        m.seq_control = 0b100;
        s.step();
        SIM_CHECK_PORT(next_upc, 3);
    })

    SIM_CASE("next", {
        m.seq_control = 0b101;
        m.upc_incr = 42;
        s.step();
        SIM_CHECK_PORT(next_upc, 42);
    })
})