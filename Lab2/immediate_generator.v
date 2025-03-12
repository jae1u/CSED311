`include "opcodes.v"

module immediate_generator (
    input [31:0] part_of_inst,
    output reg [31:0] imm_gen_out);

    always @(*) begin
        case (part_of_inst[6:0])
            `STORE : imm_gen_out = {{20{part_of_inst[31]}}, part_of_inst[31:25], part_of_inst[11:7]};
            `JAL : imm_gen_out = {{11{part_of_inst[31]}}, part_of_inst[31], part_of_inst[19:12], part_of_inst[20], part_of_inst[30:21], 1'b0};
            `BRANCH : imm_gen_out = {{19{part_of_inst[31]}}, part_of_inst[31], part_of_inst[7], part_of_inst[30:25], part_of_inst[11:8], 1'b0};
            default: imm_gen_out = {{20{part_of_inst[31]}}, part_of_inst[31:20]};
        endcase
    end

endmodule
