#include "Simulator.h"
#include "Vmy_nand.h"

COMBI_SIMULATOR(MyNand, my_nand, {
    m.a = 0b0101;
    m.b = 0b1001;
    step();
    check(0b11111111111111111111111111111110, m.c, "0b0101 nand 0b1001");
    m.a = 0;
    m.b = 0;
})