`include "def_control_signal.v"
`include "def_aluctl.v"

module alu_control (
    input [1:0] alu_op,
    input [3:0] funct,
    output reg [3:0] alu_ctl
);
    always @(*) begin
        case (alu_op)
            `ALUOP_RTYPE: begin
                case (funct)
                    4'b0000: alu_ctl = `ALUCTL_ADD;
                    4'b0010: alu_ctl = `ALUCTL_LESS_THAN;
                    4'b0011: alu_ctl = `ALUCTL_LESS_THAN_U;
                    4'b0111: alu_ctl = `ALUCTL_AND;
                    4'b0110: alu_ctl = `ALUCTL_OR;
                    4'b0100: alu_ctl = `ALUCTL_XOR;
                    4'b0001: alu_ctl = `ALUCTL_SHIFT_LEFT;
                    4'b0101: alu_ctl = `ALUCTL_SHIFT_RIGHT_LOGICAL;
                    4'b1101: alu_ctl = `ALUCTL_SHIFT_RIGHT_ARITHMETIC;
                    4'b1000: alu_ctl = `ALUCTL_SUB;
                    default: alu_ctl = 0;
                endcase
            end
            `ALUOP_ITYPE: begin
                case (funct[2:0])
                    3'b000: alu_ctl = `ALUCTL_ADD;
                    3'b010: alu_ctl = `ALUCTL_LESS_THAN;
                    3'b011: alu_ctl = `ALUCTL_LESS_THAN_U;
                    3'b111: alu_ctl = `ALUCTL_AND;
                    3'b110: alu_ctl = `ALUCTL_OR;
                    3'b100: alu_ctl = `ALUCTL_XOR;
                    3'b001: alu_ctl = `ALUCTL_SHIFT_LEFT;
                    3'b101: begin
                        if (funct[3]) alu_ctl = `ALUCTL_SHIFT_RIGHT_ARITHMETIC;
                        else alu_ctl = `ALUCTL_SHIFT_RIGHT_LOGICAL;
                    end                    
                    default: alu_ctl = 0;
                endcase
            end
            `ALUOP_ADD: alu_ctl = `ALUCTL_ADD;
            `ALUOP_B: begin
                case (funct[2:0])
                    3'b000: alu_ctl = `ALUCTL_EQUAL;
                    3'b001: alu_ctl = `ALUCTL_NOT_EQUAL;
                    3'b100: alu_ctl = `ALUCTL_LESS_THAN;
                    3'b110: alu_ctl = `ALUCTL_LESS_THAN_U;
                    3'b101: alu_ctl = `ALUCTL_GREATER_EQUAL;
                    3'b111: alu_ctl = `ALUCTL_GREATER_EQUAL_U;
                    default: alu_ctl = 0;
                endcase
            end
            default: alu_ctl = 0;
        endcase
    end
endmodule
