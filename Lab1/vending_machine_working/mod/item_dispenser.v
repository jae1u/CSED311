`include "vending_machine_def.v"

module item_dispenser (
    input clk,
    input reset_n,
    input [`kNumItems-1:0] i_select_item,
    input [`kNumItems-1:0] o_available_item,
    input [`kTotalBits-1:0] balance,
    output reg [`kNumItems-1:0] o_output_item,
    output reg [`kTotalBits-1:0] cost);

    wire [`kTotalBits-1:0] item_price [`kNumItems-1:0]; // Price of each item
    assign item_price[0] = 400;
    assign item_price[1] = 500;
    assign item_price[2] = 1000;
    assign item_price[3] = 2000;

    reg [`kNumItems-1:0] selected;
    reg [`kTotalBits-1:0] selected_cost;

    reg [`kTotalBits-1:0] prev_balance;
    reg [`kNumItems-1:0] prev_available_item;

    always @(posedge clk) begin
        if (!reset_n) begin
            prev_balance <= 0;
            prev_available_item <= 0;
        end
        else begin
            prev_balance <= balance;
            prev_available_item <= o_available_item;
        end
    end

    always @(*) begin
        // determine o_output_items
        selected = i_select_item & prev_available_item;
        selected_cost = selected[0] * item_price[0] + selected[1] * item_price[1] + selected[2] * item_price[2] + selected[3] * item_price[3];
        if (prev_balance >= selected_cost) begin
            o_output_item = selected;
        end
        else begin
            o_output_item = 0;
        end
    end

    reg [`kNumItems-1:0] selected2;
    reg [`kTotalBits-1:0] selected_cost2;

    always @(*) begin
        // determine cost
        selected2 = i_select_item & o_available_item;
        selected_cost2 = selected2[0] * item_price[0] + selected2[1] * item_price[1] + selected2[2] * item_price[2] + selected2[3] * item_price[3];
        if (balance >= selected_cost2) begin
            cost = selected_cost2;
        end
        else begin
            cost = 0;
        end
    end

endmodule

