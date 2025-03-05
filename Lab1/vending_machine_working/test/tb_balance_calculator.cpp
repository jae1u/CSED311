#include "Simulator.h"
#include "Vbalance_calculator.h"

SEQ_SIMULATOR(BalanceCalculator, balance_calculator, {
    // test - reset
    m.reset_n = 1;
    clock();
    m.reset_n = 0;
    clock();
    check(0, m.balance, "reset");
    m.reset_n = 1;

    // test - 100won
    m.i_input_coin = 0b001;
    clock();
    m.i_input_coin = 0;
    clock();
    check(100, m.balance, "100won");

    // test - 200won
    m.i_input_coin = 0b001;
    clock();
    m.i_input_coin = 0;
    clock();
    check(200, m.balance, "200won");

    // test - 700won
    m.i_input_coin = 0b010;
    clock();
    m.i_input_coin = 0;
    clock();
    check(700, m.balance, "700won");

    // test - 1700won
    m.i_input_coin = 0b100;
    clock();
    m.i_input_coin = 0;
    clock();
    check(1700, m.balance, "1700won");

    // test - multiple coins
    m.i_input_coin = 0b101; // + 1100won
    clock();
    m.i_input_coin = 0;
    clock();
    check(2800, m.balance, "2800won");

    // test - cost
    m.cost = 100;
    clock();
    check(2700, m.balance, "-100won");

    // test - bigger cost
    m.cost = 2700;
    clock();
    check(0, m.balance, "-2700won");
})
