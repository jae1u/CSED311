module timer (
    input clk,
    input reset_n,
    output reg trigger
);
    reg signed [31:0] counter;

    always @(posedge clk) begin
        if (!reset_n)
            counter = 100;
        else if (counter >= 0)
            --counter;  // counter goes down to -1

        if (counter == 0)
            trigger = 1;
        else
            trigger = 0;
    end
endmodule