#include "Simulator.h"
#include "defs.h"
#include "Vsimple_vending_machine.h"

SEQ_SIMULATOR(SimpleVendingMachine, simple_vending_machine, {
    step();
    
    // test - reset
    m.reset_n = 1;
    clock();
    m.reset_n = 0;
    clock();
    check(ITEM_NONE, m.o_available_item, "reset.o_available_item");
    check(ITEM_NONE, m.o_output_item, "reset.o_output_item");
    m.reset_n = 1;

    // test - put 100won coin
    //  => balance = 100, nothing available
    m.i_input_coin = COIN_100;
    clock();
    check(ITEM_NONE, m.o_available_item, "100won.o_available_item");
    check(ITEM_NONE, m.o_output_item, "100won.o_output_item");
    m.i_input_coin = COIN_NONE;

    // test - put 100won coin x 3
    //  => balane = 400, item A available
    m.i_input_coin = COIN_100;
    clock();
    clock();
    clock();
    check(ITEM_A, m.o_available_item, "400won.o_available_item");
    check(ITEM_NONE, m.o_output_item, "400won.o_output_item");
    m.i_input_coin = COIN_NONE;

    // test - put 100won coin
    //  => balance = 500, item A & B available
    m.i_input_coin = COIN_100;
    clock();
    check(ITEM_A | ITEM_B, m.o_available_item, "500won.o_available_item");
    check(ITEM_NONE, m.o_output_item, "500won.o_output_item");
    m.i_input_coin = COIN_NONE;

    // test - put 500won coin
    //  => balance = 1000, item A & B & C available
    m.i_input_coin = COIN_500;
    clock();
    check(ITEM_A | ITEM_B | ITEM_C, m.o_available_item, "1000won.o_available_item");
    check(ITEM_NONE, m.o_output_item, "1000won.o_output_item");
    m.i_input_coin = COIN_NONE;

    // test - put 1000won coin
    //  => balance = 2000, all items available
    m.i_input_coin = COIN_1000;
    clock();
    check(ITEM_A | ITEM_B | ITEM_C | ITEM_D, m.o_available_item, "2000won.o_available_item");
    check(ITEM_NONE, m.o_output_item, "2000won.o_output_item");
    m.i_input_coin = COIN_NONE;

    // test - buy item A
    //  => balance = 1600, item A,B,C available, item A gets dispensed
    m.i_select_item = ITEM_A;
    clock();
    check(ITEM_A | ITEM_B | ITEM_C, m.o_available_item, "buyA.o_available_item");
    check(ITEM_A, m.o_output_item, "buyA.o_output_item");
    m.i_select_item = ITEM_NONE;

    // test - buy item B
    //  => balance = 1100, item A,B,C available, item B gets dispensed
    m.i_select_item = ITEM_B;
    clock();
    check(ITEM_A | ITEM_B | ITEM_C, m.o_available_item, "buyB.o_available_item");
    check(ITEM_B, m.o_output_item, "buyB.o_output_item");
    m.i_select_item = ITEM_NONE;

    // test - buy item C
    //  => balance = 100, no itmes available, item C gets dispensed
    m.i_select_item = ITEM_C;
    clock();
    check(ITEM_NONE, m.o_available_item, "buyC.o_available_item");
    check(ITEM_C, m.o_output_item, "buyC.o_output_item");
    m.i_select_item = ITEM_NONE;

    // test - put 100 & 500won coins
    //  => balance = 700, item A,B available
    m.i_input_coin = COIN_100 | COIN_500;
    clock();
    check(ITEM_A | ITEM_B, m.o_available_item, "input100,500.o_available_item");
    check(ITEM_NONE, m.o_output_item, "input100,500.o_output_item");
    m.i_input_coin = COIN_NONE;
    
    // test - put 100 & 500 & 1000won coins
    //  => balance = 2300, all items available
    m.i_input_coin = COIN_100 | COIN_500 | COIN_1000;
    clock();
    check(ITEM_A | ITEM_B | ITEM_C | ITEM_D, m.o_available_item, "input100,500,1000.o_available_item");
    check(ITEM_NONE, m.o_output_item, "input100,500,1000.o_output_item");
    m.i_input_coin = COIN_NONE;

    // test - buy item D
    //  => balance = 300, no items available
    m.i_select_item = ITEM_D;
    clock();
    check(ITEM_NONE, m.o_available_item, "buyD.o_available_item");
    check(ITEM_D, m.o_output_item, "buyD.o_output_item");
    m.i_select_item = ITEM_NONE;

    m.i_input_coin = COIN_1000;
    for (int i = 0; i < 5; ++i) {
        clock();
    }   // balance = 5300
    m.i_input_coin = COIN_NONE;
    
    // test - buy item A & B
    //  => balance = 4400, all items available, item A,B gets dispensed
    m.i_select_item = ITEM_A | ITEM_B;
    clock();
    check(ITEM_A | ITEM_B | ITEM_C | ITEM_D, m.o_available_item, "buyAB.o_available_item");
    check(ITEM_A | ITEM_B, m.o_output_item, "buyAB.o_output_item");
    m.i_select_item = ITEM_NONE;

    // test - buy all items
    //  => balance = 500, item A,B available, all items gets dispensed
    m.i_select_item = ITEM_A | ITEM_B | ITEM_C | ITEM_D;
    clock();
    check(ITEM_A | ITEM_B, m.o_available_item, "buyABCD.o_available_item");
    check(ITEM_A | ITEM_B | ITEM_C | ITEM_D, m.o_output_item, "buyABCD.o_output_item");
    m.i_select_item = ITEM_NONE;

    // test - try buying all items 
    //  => invalid, balance = 500, item A,B available, no items gets dispensed
    m.i_select_item = ITEM_A | ITEM_B | ITEM_C | ITEM_D;
    clock();
    check(ITEM_A | ITEM_B, m.o_available_item, "buyABCDNoMoney.o_available_item");
    check(ITEM_NONE, m.o_output_item, "buyABCDNoMoney.o_output_item");
    m.i_select_item = ITEM_NONE;
})