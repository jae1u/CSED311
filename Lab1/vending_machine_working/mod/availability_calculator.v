`include "vending_machine_def.v"

module availability_calculator (
    input [31:0] balance,
    output reg [3:0] availability
);
    wire [31:0] item_price [`kNumItems-1:0];
    assign item_price[0] = 400;
    assign item_price[1] = 500;
    assign item_price[2] = 1000;
    assign item_price[3] = 2000;

    always @(*) begin
        availability = 4'b0000;
        if (balance >= item_price[0])
            availability = availability | 4'b0001;

        if (balance >= item_price[1])
            availability = availability | 4'b0010;

        if (balance >= item_price[2])
            availability = availability | 4'b0100;

        if (balance >= item_price[3])
            availability = availability | 4'b1000;
    end
endmodule