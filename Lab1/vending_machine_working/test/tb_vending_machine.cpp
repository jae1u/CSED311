#include "Simulator.h"
#include "Vvending_machine.h"


class VendingMachineSimulator : public SeqSimulator<Vvending_machine> {
public:
    VendingMachineSimulator() : SeqSimulator("vending_machine") {};
    virtual void body() {
        step(2);

        // reset
        m.reset_n = 1;
        step(2);
        m.reset_n = 0;
        step(2);

        // initial state - no coins inserted
        m.reset_n = 1;
        m.i_input_coin = 0;
        m.i_select_item = 0;
        m.i_trigger_return = 0;
        step(4);
        check(0, m.o_available_item, "noCoin.o_available_item");

        // available items test

        // 400won -> first item available
        insert_coin(1);
        insert_coin(1);
        insert_coin(1);
        m.i_input_coin = 1;
        step();
        m.i_input_coin = 0;
        check(0b0001, m.o_available_item, "400won.o_available_item");
        step();

        // 500won -> second item also available
        m.i_input_coin = 1;
        step();
        m.i_input_coin = 0;
        check(0b0011, m.o_available_item, "500won.o_available_item");
        step();

        // 1000 won -> third item also available
        insert_coin(1);
        insert_coin(1);
        insert_coin(1);
        insert_coin(1);
        m.i_input_coin = 1;
        step();
        m.i_input_coin = 0;
        check(0b0111, m.o_available_item, "1000won.o_available_item");
        step();

        // 2000won -> fourth item also avilable
        insert_coin(1);
        insert_coin(1);
        insert_coin(1);
        insert_coin(1);
        insert_coin(1);
        insert_coin(1);
        insert_coin(1);
        insert_coin(1);
        insert_coin(1);
        m.i_input_coin = 1;
        step();
        m.i_input_coin = 0;
        check(0b1111, m.o_available_item, "2000won.o_available_item");
        step();

        step(3);
        m.reset_n = 0;
        step();
        m.reset_n = 1;
        step();

        // 100 won return
        m.i_input_coin = 0b001;
        step();
        m.i_input_coin = 0;
        m.i_trigger_return = 1;
        step(6);
        m.i_trigger_return = 0;
        check(0b001, m.o_return_coin, "return100won.o_return_coin");
        step();

        m.reset_n = 0;
        step();
        m.reset_n = 1;
        // (step missing in original testbench)

        // 500 won return
        m.i_input_coin = 0b010;
        step();
        m.i_input_coin = 0;
        m.i_trigger_return = 1;
        step(6);
        m.i_trigger_return = 0;
        check(0b010, m.o_return_coin, "return500won.o_return_coin");
        step();

        m.reset_n = 0;
        step();
        m.reset_n = 1;

        // 1000 won return
        m.i_input_coin = 0b100;
        step();
        m.i_input_coin = 0;
        m.i_trigger_return = 1;
        step(6);
        m.i_trigger_return = 0;
        check(0b100, m.o_return_coin, "return1000won.o_return_coin");
        step();
    }

    void insert_coin(int val) {
        m.i_input_coin = val;
        step();
        m.i_input_coin = 0;
        step();
    }
};


int main(int argc, char** argv) {
    VendingMachineSimulator s;
    // return s.main();
}