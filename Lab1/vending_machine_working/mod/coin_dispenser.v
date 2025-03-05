`include "vending_machine_def.v"

module coin_dispenser (
    input clk,
    input ret,
    input reset_n,
    input [31:0] init_balance,
    output reg [`kNumCoins-1:0] o_return_coin,
    output reg done
);
    reg [31:0] balance;
    reg active;
    wire trigger;

    assign trigger = clk & ret;

    always @(posedge trigger) begin
        balance = init_balance;
        active = 1;
    end

    always @(posedge clk) begin
        if (active) begin
            if (balance == 0)

        end

        if (balance >= 1600)
            o_return_coin <= 3'b111;
        else if (balance >= 1500)
            o_return_coin <= 3'b110;
        else if (balance >= 1000)
            o_return_coin <= 3'b100;
        else if (balance >= 600)
            o_return_coin <= 3'b011;
        else if (balance >= 500)
            o_return_coin <= 3'b010;
        else if (balance >= 100)
            o_return_coin <= 3'b001;
        else
            o_return_coin <= 3'b000;
    end
endmodule
