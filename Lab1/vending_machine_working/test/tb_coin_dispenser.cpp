#include "Simulator.h"
#include "defs.h"
#include "Vcoin_dispenser.h"

SEQ_SIMULATOR(CoinDispenser, coin_dispenser, {
    // test - initial state
    reset();
    check(0, m.o_return_coin, "reset.o_return_coin");
    check(0, m.done, "reset.done");

    // test - balance 0
    //  -> return nothing
    m.balance = 0;
    m.ret = 1;
    clock(3);
    check(COIN_NONE, m.o_return_coin, "0won.o_return_coin");
    check(0, m.done, "0won.done");
    clock();
    check(1, m.done, "0won_done.done");
    m.balance = 0;
    m.ret = 0;
    reset();
    
    // test - balance 100
    //  -> return 100coin x1
    m.balance = 100;
    m.ret = 1;
    clock(3);
    check(COIN_100, m.o_return_coin, "100won.o_return_coin");
    check(0, m.done, "100won.done");
    clock();
    check(COIN_NONE, m.o_return_coin, "100won_end.o_return_coin");
    check(0, m.done, "100won_end.done");
    clock();
    check(1, m.done, "100won_done.done");
    m.balance = 0;
    m.ret = 0;
    reset();

    // test - balance 500
    //  -> return 500coin x1
    m.balance = 500;
    m.ret = 1;
    clock(3);
    check(COIN_500, m.o_return_coin, "500won.o_return_coin");
    check(0, m.done, "500won.done");
    clock();
    check(COIN_NONE, m.o_return_coin, "500won_end.o_return_coin");
    check(0, m.done, "500won_end.done");
    clock();
    check(1, m.done, "500won_done.done");
    m.balance = 0;
    m.ret = 0;
    reset();

    // test - balance 1000
    //  -> return 1000coin x1
    m.balance = 1000;
    m.ret = 1;
    clock(3);
    check(COIN_1000, m.o_return_coin, "1000won.o_return_coin");
    check(0, m.done, "1000won.done");
    clock();
    check(COIN_NONE, m.o_return_coin, "1000won_end.o_return_coin");
    check(0, m.done, "1000won_end.done");
    clock();
    check(1, m.done, "1000won_done.done");
    m.balance = 0;
    m.ret = 0;
    reset();

    // test - balance 600
    //  -> return 100coin x1, 500coin x1
    m.balance = 600;
    m.ret = 1;
    clock(3);
    check(COIN_100 | COIN_500, m.o_return_coin, "600won.o_return_coin");
    check(0, m.done, "600won.done");
    clock();
    check(COIN_NONE, m.o_return_coin, "600won_end.o_return_coin");
    check(0, m.done, "600won_end.done");
    clock();
    check(1, m.done, "600won_done.done");
    m.balance = 0;
    m.ret = 0;
    reset();

    // test - balance 1100
    //  -> return 100coin x1, 1000coin x1
    m.balance = 1100;
    m.ret = 1;
    clock(3);
    check(COIN_100 | COIN_1000, m.o_return_coin, "1100won.o_return_coin");
    check(0, m.done, "1100won.done");
    clock();
    check(COIN_NONE, m.o_return_coin, "1100won_end.o_return_coin");
    check(0, m.done, "1100won_end.done");
    clock();
    check(1, m.done, "1100won_done.done");
    m.balance = 0;
    m.ret = 0;
    reset();

    // test - balance 1500
    //  -> return 500coin x1, 1000coin x1
    m.balance = 1500;
    m.ret = 1;
    clock(3);
    check(COIN_500 | COIN_1000, m.o_return_coin, "1500won.o_return_coin");
    check(0, m.done, "1500won.done");
    clock();
    check(COIN_NONE, m.o_return_coin, "1500won_end.o_return_coin");
    check(0, m.done, "1500won_end.done");
    clock();
    check(1, m.done, "1500won_done.done");
    m.balance = 0;
    m.ret = 0;
    reset();

    // test - balance 1600
    //  -> return 100coin x1, 500coin x1, 1000coin x1
    m.balance = 1600;
    m.ret = 1;
    clock(3);
    check(COIN_100 | COIN_500 | COIN_1000, m.o_return_coin, "1600won.o_return_coin");
    check(0, m.done, "1600won.done");
    clock();
    check(COIN_NONE, m.o_return_coin, "1600won_end.o_return_coin");
    check(0, m.done, "1600won_end.done");
    clock();
    check(1, m.done, "1600won_done.done");
    m.balance = 0;
    m.ret = 0;
    reset();

    // test - balance 200
    //  -> return 100coin x2
    m.balance = 200;
    m.ret = 1;
    clock(3);
    check(COIN_100, m.o_return_coin, "200won_1.o_return_coin");
    check(0, m.done, "200won_1.done");
    clock();
    check(COIN_100, m.o_return_coin, "200won_2.o_return_coin");
    check(0, m.done, "200won_2.done");
    clock();
    check(COIN_NONE, m.o_return_coin, "200won_end");
    check(0, m.done, "200won_end.done");
    clock();
    check(1, m.done, "200won_done.done");
    m.balance = 0;
    m.ret = 0;
    reset();

    // test - balance 700
    //  -> return 100coin x2, 500coin x1
    m.balance = 700;
    m.ret = 1;
    clock(3);
    check(COIN_100 | COIN_500, m.o_return_coin, "700won_1.o_return_coin");
    check(0, m.done, "700won_1.done");
    clock();
    check(COIN_100, m.o_return_coin, "700won_2.o_return_coin");
    check(0, m.done, "700won_2.done");
    clock();
    check(COIN_NONE, m.o_return_coin, "700won.end");
    check(0, m.done, "700won_end.done");
    clock();
    check(1, m.done, "700won_done.done");
    m.balance = 0;
    m.ret = 0;
    reset();
})