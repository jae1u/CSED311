`include "alu_func.v"

module ALU (
    input [3:0] alu_op,
    input signed [31:0] alu_in_1,
    input signed [31:0] alu_in_2,
    output reg [31:0] alu_result);

    reg alu_bcond; // DEPRECATED: ALU branch condition output

    always @(*) begin
        case(alu_op)
            `FUNC_ADD: begin
                alu_result = alu_in_1 + alu_in_2;
                alu_bcond = 0;
            end
            `FUNC_SUB: begin
                alu_result = alu_in_1 - alu_in_2;
                alu_bcond = 0;
            end
            `FUNC_BEQ: begin
                alu_result = 0;
                alu_bcond = (alu_in_1 == alu_in_2);
            end
            `FUNC_BNE: begin
                alu_result = 0;
                alu_bcond = (alu_in_1 != alu_in_2);
            end
            `FUNC_BLT: begin
                alu_result = 0;
                alu_bcond = (alu_in_1 < alu_in_2);
            end
            `FUNC_BGE: begin
                alu_result = 0;
                alu_bcond = (alu_in_1 >= alu_in_2);
            end
            `FUNC_XOR: begin
                alu_result = alu_in_1 ^ alu_in_2;
                alu_bcond = 0;
            end
            `FUNC_OR: begin
                alu_result = alu_in_1 | alu_in_2;
                alu_bcond = 0;
            end
            `FUNC_AND: begin
                alu_result = alu_in_1 & alu_in_2;
                alu_bcond = 0;
            end
            `FUNC_LLS: begin
                alu_result = alu_in_1 << alu_in_2;
                alu_bcond = 0;
            end
            `FUNC_LRS: begin
                alu_result = alu_in_1 >> alu_in_2;
                alu_bcond = 0;
            end
            `FUNC_ARS: begin
                alu_result = alu_in_1 >>> alu_in_2;
                alu_bcond = 0;
            end
            default: begin
                alu_result = 0;
                alu_bcond = 0;
            end
        endcase
    end

endmodule
