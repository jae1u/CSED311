#include "Simulator.h"
#include "Vitem_dispenser.h"

SEQ_SIMULATOR(ItemDispenser, item_dispenser, {
    // test - reset
    m.reset_n = 0;
    clock();
    m.reset_n = 1;
    clock();
    check(0, m.o_output_item, "reset.o_output_item");
    check(0, m.cost, "reset.o_output_item");

    // test - 400won, no selection, no available items -> invalid
    m.i_select_item = 0b0000;
    m.o_available_item = 0b0000;
    m.balance = 400;
    clock();
    check(0, m.o_output_item, "400wonNoSelNoAvail.o_output_item");
    check(0, m.cost, "400wonNoSelNoAvail.cost");

    // test - 400won, select item 1, no available items -> invalid
    m.i_select_item = 0b0001;
    m.o_available_item = 0b0000;
    m.balance = 400;
    clock();
    check(0, m.o_output_item, "400wonSel1NoAvail.o_output_item");
    check(0, m.cost, "400wonSel1NoAvail.cost");

    // test - 400won, select item 1, all but item 1 available -> invalid
    m.i_select_item = 0b0001;
    m.o_available_item = 0b1110;
    m.balance = 400;
    clock();
    check(0, m.o_output_item, "400wonSel1Avail234.o_output_item");
    check(0, m.cost, "400wonSel1Avail234.cost");

    // test - 400won, select item 1, all available -> purchase 1
    m.i_select_item = 0b0001;
    m.o_available_item = 0b1111;
    m.balance = 400;
    clock();
    check(0b0001, m.o_output_item, "400wonSel1AllAvail.o_output_item");
    check(400, m.cost, "400wonSel1AllAvail.cost");

    // test - 800won, select item 1 & 2, all available -> invalid
    m.i_select_item = 0b0011;
    m.o_available_item = 0b1111;
    m.balance = 800;
    clock();
    check(0, m.o_output_item, "800wonSel12AllAvail.o_output_item");
    check(0, m.cost, "800wonSel12AllAvail.cost");

    // test - 1000won, select item 1 & 2, all available -> purchase 1 & 2
    m.i_select_item = 0b0011;
    m.o_available_item = 0b1111;
    m.balance = 1000;
    clock();
    check(0b0011, m.o_output_item, "1000wonSel12AllAvail.o_output_item");
    check(900, m.cost, "1000wonSel12AllAvail.cost");

    // test - 4000won, select all, all available -> purchase all
    m.i_select_item = 0b1111;
    m.o_available_item = 0b1111;
    m.balance = 4000;
    clock();
    check(0b1111, m.o_output_item, "4000wonAllSelAllAvail.o_output_item");
    check(3900, m.cost, "4000wonAllSelAllAvail.cost");
})
