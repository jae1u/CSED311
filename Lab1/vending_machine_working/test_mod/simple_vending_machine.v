`include "vending_machine_def.v"

// Simple vending machine without timeout and return support
// (balance_calculator + availability_calculator + item_dispenser)

module simple_vending_machine (
    input clk,
    input reset_n,
    input [`kNumCoins-1:0] i_input_coin,
    input [`kNumItems-1:0] i_select_item,
    output [`kNumItems-1:0] o_available_item,
    output [`kNumItems-1:0] o_output_item
);
    wire [`kTotalBits-1:0] cost, balance;
    wire [`kNumItems-1:0] available_items;

    assign o_available_item = available_items;
    
    balance_calculator bc(
        .clk(clk),
        .reset_n(reset_n),
        .i_input_coin(i_input_coin),
        .cost(cost),
        .balance(balance)
    );
    availability_calculator ac(
        .balance(balance),
        .o_available_item(available_items)
    );
    item_dispenser d(
        .clk(clk),
        .reset_n(reset_n),
        .i_select_item(i_select_item),
        .o_available_item(available_items),
        .balance(balance),
        .o_output_item(o_output_item),
        .cost(cost)
    );
endmodule