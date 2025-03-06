`include "vending_machine_def.v"

module availability_calculator (
    input [`kTotalBits-1:0] balance,
    output reg [`kNumItems-1:0] o_available_item);

    wire [`kTotalBits-1:0] item_price [`kNumItems-1:0]; // Price of each item
    assign item_price[0] = 400;
    assign item_price[1] = 500;
    assign item_price[2] = 1000;
    assign item_price[3] = 2000;

    always @(*) begin
        o_available_item = 4'b0000;
        if (balance >= item_price[0])
            o_available_item = o_available_item | 4'b0001;

        if (balance >= item_price[1])
            o_available_item = o_available_item | 4'b0010;

        if (balance >= item_price[2])
            o_available_item = o_available_item | 4'b0100;

        if (balance >= item_price[3])
            o_available_item = o_available_item | 4'b1000;
    end

endmodule
