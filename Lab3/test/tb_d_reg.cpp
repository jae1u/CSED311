#include "Simulator.h"
#include "Vd_reg.h"

SIM_SEQ(d_reg, {
    SIM_SEQ_SETUP({
        m.data = 0;
        m.write = 0;
        m.reset = 1;
        m.clk = 0;
        s.tick();
        m.reset = 0;
    })

    SIM_SEQ_CASE("initial state", {
        SIM_CHECK_PORT(out, 0);
    })

    SIM_SEQ_CASE("write = 0", {
        m.data = 42;
        s.tick();
        SIM_CHECK_PORT(out, 0);
    })

    SIM_SEQ_CASE("write = 1", {
        m.data = 42;
        m.write = 1;
        s.tick();
        SIM_CHECK_PORT(out, 42);
    })
})