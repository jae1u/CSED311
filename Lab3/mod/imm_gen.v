`include "def_opcode.v"

module imm_gen (
    input [31:0] instr,
    output reg [31:0] imm
);
    always @(*) begin
        case (instr[6:0])
            // I-Type
            `ARITHMETIC_IMM: begin
                if (instr[14:12] == `FUNCT3_SLL || instr[14:12] == `FUNCT3_SRL || instr[14:12] == `FUNCT3_SRA) begin
                    imm = {{27{1'b0}}, instr[24:20]};
                end
                else imm = {{21{instr[31]}}, instr[30:20]};
            end
            `LOAD, `JALR: imm = {{21{instr[31]}}, instr[30:20]};
            // S-Type
            `STORE: imm = {{21{instr[31]}}, instr[30:25], instr[11:7]};
            // B-Type
            `BRANCH: imm = {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0};
            // J-Type
            `JAL: imm = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};
            default: imm = 0;
        endcase
    end
endmodule
