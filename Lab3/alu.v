`include "def_aluctl.v"

module alu #(
    SIZE = 32
) (
    input signed [SIZE-1:0] a,
    input signed [SIZE-1:0] b,
    input [3:0] alu_ctl,
    output reg [SIZE-1:0] result
);

always @(*) begin
    case (alu_ctl)
        `ALUCTL_AND: begin
            result = a & b;
        end
        `ALUCTL_OR: begin
            result = a | b;
        end
        `ALUCTL_XOR: begin
            result = a ^ b;
        end
        `ALUCTL_ADD: begin
            result = a + b;
        end
        `ALUCTL_SUB: begin
            result = a - b;
        end
        `ALUCTL_SHIFT_LEFT: begin
            result = a << b;
        end
        `ALUCTL_SHIFT_RIGHT_LOGICAL: begin
            result = a >> b;
        end
        `ALUCTL_SHIFT_RIGHT_ARITHMETIC: begin
            result = a >>> b;
        end
        `ALUCTL_LESS_THAN: begin
            result = {{(SIZE-1){1'b0}}, a < b};
        end
        `ALUCTL_GREATER_EQUAL: begin
            result = {{(SIZE-1){1'b0}}, a >= b};
        end
        `ALUCTL_LESS_THAN_U: begin
            result = {{(SIZE-1){1'b0}}, $unsigned(a) < $unsigned(b)};
        end
        `ALUCTL_GREATER_EQUAL_U: begin
            result = {{(SIZE-1){1'b0}}, $unsigned(a) >= $unsigned(b)};
        end
        `ALUCTL_EQUAL: begin
            result = {{(SIZE-1){1'b0}}, a == b};
        end
        `ALUCTL_NOT_EQUAL: begin
            result = {{(SIZE-1){1'b0}}, a != b};
        end
        default: begin
            result = 0;
        end
    endcase
end
    
endmodule
