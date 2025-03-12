`include "opcodes.v"
`include "alu_func.v"

module alu_control_unit (
    input [16:0] part_of_inst,
    output reg [3:0] alu_op);

    wire [6:0] func7 = part_of_inst[16:10];
    wire [2:0] func3 = part_of_inst[9:7];
    wire [6:0] opcode = part_of_inst[6:0];

    always @(*) begin
        case (opcode)
            `ARITHMETIC: begin
                case (func3)
                    `FUNCT3_ADD: alu_op = ((func7 == `FUNCT7_OTHERS) ? `FUNC_ADD : `FUNC_SUB);
                    `FUNCT3_XOR: alu_op = `FUNC_XOR;
                    `FUNCT3_OR: alu_op = `FUNC_OR;
                    `FUNCT3_AND: alu_op = `FUNC_AND;
                    `FUNCT3_SRL: alu_op = ((func7 == `FUNCT7_OTHERS) ? `FUNC_LRS : `FUNC_ARS);
                    default: alu_op = `FUNC_ADD;
                endcase
            end
            `ARITHMETIC_IMM: begin
                case (func3)
                    `FUNCT3_ADD: alu_op = `FUNC_ADD;
                    `FUNCT3_SLL: alu_op = `FUNC_LLS;
                    `FUNCT3_XOR: alu_op = `FUNC_XOR;
                    `FUNCT3_OR: alu_op = `FUNC_OR;
                    `FUNCT3_AND: alu_op = `FUNC_AND;
                    `FUNCT3_SRL: alu_op = ((func7 == `FUNCT7_OTHERS) ? `FUNC_LRS : `FUNC_ARS);
                    default: alu_op = `FUNC_ADD;
                endcase
            end
            `BRANCH: begin
                case (func3)
                    `FUNCT3_BEQ: alu_op = `FUNC_BEQ;
                    `FUNCT3_BNE: alu_op = `FUNC_BNE;
                    `FUNCT3_BLT: alu_op = `FUNC_BLT;
                    `FUNCT3_BGE: alu_op = `FUNC_BGE;
                    default: alu_op = `FUNC_ADD;
                endcase
            end
            default: alu_op = `FUNC_ADD;
        endcase
    end

endmodule
