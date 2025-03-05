`include "vending_machine_def.v"

module coin_dispenser (
    input clk,
    input i_trigger_return,
    input [`kTotalBits-1:0] balance,
    output reg [`kNumCoins-1:0] o_return_coin,
    output reg [`kTotalBits-1:0] coin_cost);

    wire [`kTotalBits-1:0] coin_value [`kNumCoins-1:0]; // Value of each coin
    assign coin_value[0] = 100;
    assign coin_value[1] = 500;
    assign coin_value[2] = 1000;

    reg trigger;

    always @(posedge clk) begin
        if (i_trigger_return && balance != 0)
            trigger <= 1;

        if (balance == 0) begin
            o_return_coin <= 3'b000;
            trigger <= 0;
        end

        if (trigger == 1) begin
            if (balance >= 1600)
                o_return_coin <= 3'b111;
            else if (balance >= 1500)
                o_return_coin <= 3'b110;
            else if (balance >= 1100)
                o_return_coin <= 3'b101;
            else if (balance >= 1000)
                o_return_coin <= 3'b100;
            else if (balance >= 600)
                o_return_coin <= 3'b011;
            else if (balance >= 500)
                o_return_coin <= 3'b010;
            else
                o_return_coin <= 3'b001;
        end
        else o_return_coin <= 3'b000;

        coin_cost <= o_return_coin[0] * coin_value[0] + o_return_coin[1] * coin_value[1] + o_return_coin[2] * coin_value[2];
    end

endmodule
