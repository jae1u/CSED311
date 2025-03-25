module d_reg #(
    parameter SIZE = 32
) (
    input reset,
    input [SIZE-1:0] data,
    input write,
    input clk,
    output reg [SIZE-1:0] out
);
    always @(posedge clk) begin
        if (reset) out <= {SIZE{1'b0}};
        else if (write) out <= data;
    end
endmodule
