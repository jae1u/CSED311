#include "Simulator.h"
#include "Vmemory.h"
#include "Vmemory_memory.h"
#include "helpers.h"


SIM_SEQ(memory, {
    SIM_SEQ_SETUP({
        m.addr = 0;
        m._testing_manual_reset = 1;
        m.write_data = 0;
        m.mem_read = 0;
        m.mem_write = 0;
        m.reset = 1;
        reset_memory<Vmemory_memory>(m.memory);
        s.tick();
        m.reset = 0;
    })

    SIM_SEQ_CASE("initial state", {
        SIM_CHECK_PORT(mem_data, 0);
    })

    SIM_SEQ_CASE("mem_read = 0", {
        init_memory<Vmemory_memory>(m.memory, {42, 84}, 0);

        s.tick();
        SIM_CHECK_PORT(mem_data, 0);

        m.addr = 4;
        s.tick();
        SIM_CHECK_PORT(mem_data, 0);
    })

    SIM_SEQ_CASE("mem_read = 1", {
        init_memory<Vmemory_memory>(m.memory, {42, 84}, 0);

        m.mem_read = 1;
        s.tick();
        SIM_CHECK_PORT(mem_data, 42);

        m.addr = 4;
        s.tick();
        SIM_CHECK_PORT(mem_data, 84);
    })
    
    SIM_SEQ_CASE("mem_write = 0", {
        m.mem_read = 1;
        m.write_data = 42;
        s.tick();
        SIM_CHECK_PORT(mem_data, 0);
    })

    SIM_SEQ_CASE("mem_write = 1", {
        m.mem_read = 1;
        m.mem_write = 1;
        m.write_data = 42;
        s.tick();
        SIM_CHECK_PORT(mem_data, 42);
    })
})