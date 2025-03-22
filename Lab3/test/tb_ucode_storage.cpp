#include "Simulator.h"
#include "Vucode_storage.h"

SIM_COMBI(ucode_storage, {
    SIM_SETUP({
        m.upc = 0;
    })

    SIM_CASE("IF", {
        m.upc = 0;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b000000000000001'101);
    })

    SIM_CASE("ID", {
        m.upc = 1;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b000000000001110'001);
    })

    SIM_CASE("REX", {
        m.upc = 2;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b000000000010000'101);
    })

    SIM_CASE("RWB", {
        m.upc = 3;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b000000001100000'000);
    })

    SIM_CASE("MemAddr", {
        m.upc = 4;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b000000010000000'010);
    })

    SIM_CASE("LDMemRead", {
        m.upc = 5;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b000000100000000'101);
    })

    SIM_CASE("LDWB", {
        m.upc = 6;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b000001001000000'000);
    })

    SIM_CASE("SDMemWrite", {
        m.upc = 7;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b000010001000000'000);
    })

    SIM_CASE("BCheck", {
        m.upc = 8;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b000100000000000'011);
    })

    SIM_CASE("Branch", {
        m.upc = 9;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b001000000000000'000);
    })

    SIM_CASE("JAL", {
        m.upc = 10;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b001000000100000'000);
    })
    
    SIM_CASE("JALR", {
        m.upc = 11;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b010000000100000'000);
    })

    SIM_CASE("IEX", {
        m.upc = 12;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b100000000000000'100);
    })

    SIM_CASE("NOP", {
        m.upc = 13;
        s.step();
        SIM_CHECK_PORT(uinstr, 0b000000001000000'000);
    })
})