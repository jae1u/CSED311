#include <string>
#include "Simulator.h"
#include "Vtimer.h"

SEQ_SIMULATOR(Timer, timer, {
    // test - initial state
    m.reset_n = 1;
    clock();
    m.reset_n = 0;
    clock();
    m.reset_n = 1;
    check(0, m.trigger, "initial");
    // counter = 100 at this moment

    // test - countdown from 100 to 0
    for (int i = 0; i < 100; ++i) {
        if (m.trigger != 0) {
            fail(std::string("timer triggered ") + std::to_string(i) + " clk after reset");
            return;
        }
        clock();
    }

    // test - timer send 1 clk pulse when counter == 0
    check(1, m.trigger, "timerTriggerHigh");
    clock();
    check(0, m.trigger, "timerTriggerLow");

    // test - timer deactivated
    clock();
    check(0, m.trigger, "timerDead");
})