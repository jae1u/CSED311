`include "vending_machine_def.v"


module coin_dispenser (
    input clk,
    input reset_n,
    input ret,
    input [`kTotalBits-1:0] balance,
    output reg [`kNumCoins-1:0] o_return_coin,
    output reg done);

    wire [`kTotalBits-1:0] coin_value [`kNumCoins-1:0]; // Value of each coin
    assign coin_value[0] = 100;
    assign coin_value[1] = 500;
    assign coin_value[2] = 1000;

    reg [`kTotalBits-1:0] change_due;
    reg [`kTotalBits-1:0] change_popped;
    reg [1:0] trigger;

    always @(posedge clk) begin
        if (!reset_n) begin
            change_due <= 0;
            change_popped <= 0;
            o_return_coin <= 0;
            trigger <= 0;
            done <= 0;
        end
        else begin
            case (trigger)
                0: begin
                    if (ret) begin
                        change_due <= balance;
                        change_popped <= 0;
                        trigger <= 1;
                    end
                end
                1: begin
                    trigger <= 2;
                end
                2: begin
                    if (change_due >= 1600) begin
                        o_return_coin <= 3'b111;
                        change_due <= change_due - 1600;
                    end
                    else if (change_due >= 1500) begin
                        o_return_coin <= 3'b110;
                        change_due <= change_due - 1500;
                    end
                    else if (change_due >= 1100) begin
                        o_return_coin <= 3'b101;
                        change_due <= change_due - 1100;
                    end
                    else if (change_due >= 1000) begin
                        o_return_coin <= 3'b100;
                        change_due <= change_due - 1000;
                    end
                    else if (change_due >= 600) begin
                        o_return_coin <= 3'b011;
                        change_due <= change_due - 600;
                    end
                    else if (change_due >= 500) begin
                        o_return_coin <= 3'b010;
                        change_due <= change_due - 500;
                    end
                    else if (change_due >= 100) begin
                        o_return_coin <= 3'b001;
                        change_due <= change_due - 100;
                    end
                    else begin
                        o_return_coin <= 3'b000;
                        trigger <= 3;
                    end
                end
                3: begin
                    done <= 1;
                end
            endcase
        end
    end
endmodule
