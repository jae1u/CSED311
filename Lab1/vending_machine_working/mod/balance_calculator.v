`include "vending_machine_def.v"

module balance_calculator (
    input clk,
    input reset_n,
    input [`kNumCoins-1:0] i_input_coin,
    input [`kTotalBits-1:0] cost,
    output reg [`kTotalBits-1:0] balance);

    wire [`kTotalBits-1:0] coin_value [`kNumCoins-1:0]; // Value of each coin
    assign coin_value[0] = 100;
    assign coin_value[1] = 500;
    assign coin_value[2] = 1000;

    always @(posedge clk) begin
        if (!reset_n)
            balance <= 0;
        else
            balance <= balance - cost + i_input_coin[0] * coin_value[0] + i_input_coin[1] * coin_value[1] + i_input_coin[2] * coin_value[2];
    end

endmodule
