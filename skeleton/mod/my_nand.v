`include "sample_def.v"

module my_nand (
    input [`kTotalBits-1:0] a,
    input [`kTotalBits-1:0] b,
    output [`kTotalBits-1:0] c
);
    assign c = ~(a & b);
endmodule