#include "Simulator.h"
#include "Vregister_file.h"

SIM_SEQ(register_file, {
    SIM_SEQ_SETUP({
        m.read_reg_1 = 0;
        m.read_reg_2 = 0;
        m.write_reg = 0;
        m.write_data = 0;
        m.reset = 1;
        m.reg_write = 0;
        s.tick();
        m.reset = 0;
    })

    SIM_SEQ_CASE("initial state", {
        SIM_CHECK_PORT(read_data_1, 0);
        SIM_CHECK_PORT(read_data_2, 0);
    })

    SIM_SEQ_CASE("read", {
        m.reg_write = 1;
        m.write_reg = 4;
        m.write_data = 42;
        s.tick();
        m.write_reg = 5;
        m.write_data = 84;
        s.tick();
        m.reg_write = 0;
        m.read_reg_1 = 4;
        m.read_reg_2 = 5;
        s.tick();
        SIM_CHECK_PORT(read_data_1, 42);
        SIM_CHECK_PORT(read_data_2, 84);
    })

    SIM_SEQ_CASE("reg_write = 0", {
        m.write_reg = 4;
        m.write_data = 42;
        s.tick();
        m.read_reg_1 = 4;
        s.tick();
        SIM_CHECK_PORT(read_data_1, 0);
    })

    SIM_SEQ_CASE("reg_write = 1", {
        m.reg_write = 1;
        m.write_reg = 4;
        m.write_data = 42;
        s.tick();
        m.reg_write = 0;
        m.read_reg_1 = 4;
        s.tick();
        SIM_CHECK_PORT(read_data_1, 42);
    })

    SIM_SEQ_CASE("x0", {
        s.tick();
        SIM_CHECK_PORT(read_data_1, 0);
        m.reg_write = 1;
        m.write_data = 42;
        s.tick();
        SIM_CHECK_PORT(read_data_1, 0);
    })

    SIM_SEQ_CASE("x2 = 0x2ffc", {
        m.read_reg_1 = 2;
        s.tick();
        SIM_CHECK_PORT(read_data_1, 0x2ffc);
    })
})