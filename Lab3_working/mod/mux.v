/* 2^N:1 K-bit mux */
module mux #(
    parameter N = 1,
    parameter K = 1
) (
    input [K-1:0] ins [2**N-1:0],
    input [N-1:0] select,
    output reg [K-1:0] out
);
    always @(*) begin
        out = ins[select];
    end
endmodule
