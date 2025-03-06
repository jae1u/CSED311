`include "vending_machine_def.v"

module timer (
    input clk,
    input reset_n,
    output reg trigger);

    reg signed [`kTotalBits-1:0] counter;

    always @(posedge clk) begin
        if (!reset_n) begin
            counter <= `kWaitTime - 1;
            trigger <= 0;
        end
        else begin
            if (counter > 0)
                counter <= counter - 1; // counter goes down to -1
            else if (counter == 0) begin
                trigger <= 1;
                counter <= counter - 1;
            end
            else
                trigger <= 0;
        end
    end

endmodule
