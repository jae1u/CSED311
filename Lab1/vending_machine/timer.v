`include "vending_machine_def.v"

module timer (
    input clk,
    input reset_n,
    output reg trigger);

    reg signed [`kTotalBits-1:0] counter;

    always @(posedge clk) begin
        if (!reset_n)
            counter <= `kWaitTime + 1;
        else if (counter > 0)
            counter <= counter - 1;

        if (counter == 0)
            trigger = 1;
        else
            trigger = 0;
    end

endmodule
