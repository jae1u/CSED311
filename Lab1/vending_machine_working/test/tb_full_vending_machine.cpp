#include "Simulator.h"
#include "defs.h"
#include "Vfull_vending_machine.h"


SEQ_SIMULATOR(FullVendingMachine, full_vending_machine, {
    // reset
    reset();
    check(ITEM_NONE, m.o_available_item, "reset.o_available_item");
    check(ITEM_NONE, m.o_output_item, "reset.o_output_item");
    check(COIN_NONE, m.o_return_coin, "reset.o_return_coin");

    // 400won -> item A available
    m.i_input_coin = COIN_100;
    clock(4);
    check(ITEM_A, m.o_available_item, "400won.o_available_item");
    check(ITEM_NONE, m.o_output_item, "400won.o_output_item");
    check(COIN_NONE, m.o_return_coin, "400won.o_return_coin");
    m.i_input_coin = COIN_NONE;

    // 500won -> item B also available
    m.i_input_coin = COIN_100;
    clock();
    check(ITEM_A | ITEM_B, m.o_available_item, "500won.o_available_item");
    check(ITEM_NONE, m.o_output_item, "500won.o_output_item");
    check(COIN_NONE, m.o_return_coin, "500won.o_return_coin");
    m.i_input_coin = COIN_NONE;

    // 1000 won -> item C also available
    m.i_input_coin = COIN_100;
    clock(5);
    check(ITEM_A | ITEM_B | ITEM_C, m.o_available_item, "1000won.o_available_item");
    check(ITEM_NONE, m.o_output_item, "1000won.o_output_item");
    check(COIN_NONE, m.o_return_coin, "1000won.o_return_coin");
    m.i_input_coin = COIN_NONE;

    // 2000won -> item D also avilable
    m.i_input_coin = COIN_100;
    clock(10);
    check(ITEM_A | ITEM_B | ITEM_C | ITEM_D, m.o_available_item, "2000won.o_available_item");
    check(ITEM_NONE, m.o_output_item, "2000won.o_output_item");
    check(COIN_NONE, m.o_return_coin, "2000won.o_return_coin");
    m.i_input_coin = COIN_NONE;

    reset();

    // 100 won return -> 100 coin x1    m.i_trigger_return = 0;
    m.i_input_coin = COIN_100;
    clock();
    m.i_input_coin = COIN_NONE;
    m.i_trigger_return = 1;
    clock(3);
    check(COIN_100, m.o_return_coin, "return100.o_return_coin");
    m.i_trigger_return = 0;

    reset();

    // 500 won return  -> 500 coin x1
    m.i_input_coin = COIN_500;
    clock();
    m.i_input_coin = COIN_NONE;
    m.i_trigger_return = 1;
    clock(3);
    check(COIN_500, m.o_return_coin, "return500.o_return_coin");
    m.i_trigger_return = 0;

    reset();

    // 1000 won return -> 1000 coin x1
    m.i_input_coin = COIN_1000;
    clock();
    m.i_input_coin = COIN_NONE;
    m.i_trigger_return = 1;
    clock(3);
    check(COIN_1000, m.o_return_coin, "return1000.o_return_coin");
    m.i_trigger_return = 0;
    
    reset();

    // 600 won return -> 100 coin x1, 500 coin x1
    m.i_input_coin = COIN_100 | COIN_500;
    clock();
    m.i_input_coin = COIN_NONE;
    m.i_trigger_return = 1;
    clock(3);
    check(COIN_100 | COIN_500, m.o_return_coin, "return600.o_return_coin");
    m.i_trigger_return = 0;

    reset();
    m.i_trigger_return = 0;

    // 1100 won return -> 100 coin x1, 1000 coin x1
    m.i_input_coin = COIN_100 | COIN_1000;
    clock();
    m.i_input_coin = COIN_NONE;
    m.i_trigger_return = 1;
    clock(3);
    check(COIN_100 | COIN_1000, m.o_return_coin, "return1100.o_return_coin");
    m.i_trigger_return = 0;

    reset();

    // 1500 won return -> 500 coin x1, 1000 coin x1
    m.i_input_coin = COIN_500 | COIN_1000;
    clock();
    m.i_input_coin = COIN_NONE;
    m.i_trigger_return = 1;
    clock(3);
    check(COIN_500 | COIN_1000, m.o_return_coin, "return1500.o_return_coin");
    m.i_trigger_return = 0;

    reset();

    // balance 1600, select item B,C
    //  -> cost: 1500, return 100coin x1, output item B,C
    m.i_input_coin = COIN_100 | COIN_500 | COIN_1000;
    clock();
    m.i_input_coin = COIN_NONE;
    m.i_select_item = ITEM_B | ITEM_C;
    clock();
    check(ITEM_B | ITEM_C, m.o_output_item, "buyBC.o_output_item");
    m.i_select_item = ITEM_NONE;
    m.i_trigger_return = 1;
    clock(3);
    check(COIN_100, m.o_return_coin, "buyBC.o_return_coin");
    clock();
    m.i_trigger_return = 0;

    // reset after coin returned
    check(COIN_NONE, m.o_return_coin, "done.o_return_coin");
    clock(2);
    check(ITEM_NONE, m.o_available_item, "done.o_available_item");
    check(ITEM_NONE, m.o_output_item, "done.3.o_output_item");
    
    reset();

    // trigger return after 100 clocks
    m.i_input_coin = COIN_500;
    clock();
    m.i_input_coin = COIN_NONE;
    if (m.o_available_item != (ITEM_A | ITEM_B)) {
        fail("timeout - invalid o_available_item output");
        return;
    }
    clock(99 + 3);
    check(COIN_500, m.o_return_coin, "timeout.1.o_return_coin");
    clock();
    check(COIN_NONE, m.o_return_coin, "timeout.2.o_return_coin");
    clock(2);
    check(ITEM_NONE, m.o_available_item, "timeout.reset.o_available_item");
    check(ITEM_NONE, m.o_output_item, "timeout.reset.o_output_item");
    check(COIN_NONE, m.o_return_coin, "timeout.reset.o_return_coin");
})
