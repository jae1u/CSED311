module Gshare(
    input [31:0] IF_pc,
    output reg [31:0] predicted_pc);

    always @(*) begin
        predicted_pc = IF_pc + 4;
    end

endmodule
