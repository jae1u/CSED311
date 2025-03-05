`include "vending_machine_def.v"

module balance_calculator (
    input clk,
    input reset_n,
    input [`kNumCoins-1:0] i_input_coin,
    input [31:0] cost,
    output reg [31:0] balance
);
    always @(posedge clk) begin
        if (!reset_n)
            balance = 0;
        else
            balance = balance - cost + i_input_coin[0] * 100 + i_input_coin[1] * 500 + i_input_coin[2] * 1000;
    end
    
endmodule
