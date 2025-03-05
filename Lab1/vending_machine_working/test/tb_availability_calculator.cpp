#include "Simulator.h"
#include "Vavailability_calculator.h"


COMBI_SIMULATOR(AvailabilityCalculator, availability_calculator, {
    // balance 0 -> nothing available
    m.balance = 0;
    step();
    check(0b0000, m.availability, "balance0");

    // balance 200 -> nothing available
    m.balance = 200;
    step();
    check(0b0000, m.availability, "balance200");

    // balance 400 -> item 1 available
    m.balance = 400;
    step();
    check(0b0001, m.availability, "balance400");

    // balance 500 -> item 2 also available
    m.balance = 500;
    step();
    check(0b0011, m.availability, "balance500");

    // balance 1000 -> item 3 also available
    m.balance = 1000;
    step();
    check(0b0111, m.availability, "balance1000");

    // balance 2000 -> item 4 available
    m.balance = 2000;
    step();
    check(0b1111, m.availability, "balance2000");
})