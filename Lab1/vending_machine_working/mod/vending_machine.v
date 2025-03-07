`include "vending_machine_def.v"

module vending_machine (
    input clk, // Clock signal
    input reset_n, // Reset signal (active-low)
    input [`kNumCoins-1:0] i_input_coin, // coin is inserted.
    input [`kNumItems-1:0] i_select_item, // item is selected.
    input i_trigger_return, // change-return is triggered
    output [`kNumItems-1:0] o_available_item, // Sign of the item availability
    output [`kNumItems-1:0] o_output_item, // Sign of the item withdrawal
    output [`kNumCoins-1:0] o_return_coin); // Sign of the coin return

    wire [`kTotalBits-1:0] balance;
    wire [`kTotalBits-1:0] cost;
    wire done;  // reset signal emitted by coin_dispenser, active-high
    wire timer_trigger;
    wire full_reset;
    wire ret;

    assign full_reset = reset_n & ~done;
    assign ret = timer_trigger | i_trigger_return;

    availability_calculator availability_calculator_module(
    .balance(balance),
    .o_available_item(o_available_item));

    balance_calculator balance_calculator_module(
    .clk(clk),
    .reset_n(full_reset),
    .i_input_coin(i_input_coin),
    .cost(cost),
    .balance(balance));

    timer timer_module(
    .clk(clk),
    .reset_n(full_reset),
    .trigger(timer_trigger)
    );

    coin_dispenser coin_dispenser_module(
    .clk(clk),
    .reset_n(full_reset),
    .ret(ret),
    .balance(balance),
    .o_return_coin(o_return_coin),
    .done(done));

    item_dispenser item_dispenser_module(
    .clk(clk),
    .reset_n(full_reset),
    .i_select_item(i_select_item),
    .o_available_item(o_available_item),
    .balance(balance),
    .o_output_item(o_output_item),
    .cost(cost));

endmodule


// below is a working version without timer
// module vending_machine (
//     input clk, // Clock signal
//     input reset_n, // Reset signal (active-low)
//     input [`kNumCoins-1:0] i_input_coin, // coin is inserted.
//     input [`kNumItems-1:0] i_select_item, // item is selected.
//     input i_trigger_return, // change-return is triggered
//     output [`kNumItems-1:0] o_available_item, // Sign of the item availability
//     output [`kNumItems-1:0] o_output_item, // Sign of the item withdrawal
//     output [`kNumCoins-1:0] o_return_coin); // Sign of the coin return

//     wire [`kTotalBits-1:0] balance;
//     wire [`kTotalBits-1:0] cost;
//     wire done;  // reset signal emitted by coin_dispenser, active-high
//     wire full_reset;
//     wire ret;

//     assign full_reset = reset_n;
//     assign ret = i_trigger_return;

//     availability_calculator availability_calculator_module(
//     .balance(balance),
//     .o_available_item(o_available_item));

//     balance_calculator balance_calculator_module(
//     .clk(clk),
//     .reset_n(full_reset),
//     .i_input_coin(i_input_coin),
//     .cost(cost),
//     .balance(balance));

//     coin_dispenser coin_dispenser_module(
//     .clk(clk),
//     .reset_n(full_reset),
//     .ret(ret),
//     .balance(balance),
//     .o_return_coin(o_return_coin),
//     .done(done));

//     item_dispenser item_dispenser_module(
//     .clk(clk),
//     .reset_n(full_reset),
//     .i_select_item(i_select_item),
//     .o_available_item(o_available_item),
//     .balance(balance),
//     .o_output_item(o_output_item),
//     .cost(cost));

// endmodule
