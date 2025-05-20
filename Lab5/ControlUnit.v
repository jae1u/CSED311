`include "opcodes.v"

module ControlUnit (
    input [6:0] part_of_inst,
    output is_jal,
    output is_jalr,
    output branch,
    output mem_read,       // MemRead
    output mem_to_reg,     // MemtoReg
    output mem_write,      // MemWrite
    output alu_src,        // ALUSrc
    output write_enable,   // RegWrite
    output pc_to_reg,
    output [6:0] alu_op,         // ALUOp
    output is_ecall);      // (ecall inst)

    assign is_jal = (part_of_inst == `JAL);
    assign is_jalr = (part_of_inst == `JALR);
    assign branch = (part_of_inst == `BRANCH);
    assign mem_read = (part_of_inst == `LOAD);
    assign mem_to_reg = (part_of_inst == `LOAD);
    assign mem_write = (part_of_inst == `STORE);
    assign alu_src = (part_of_inst != `ARITHMETIC) && (part_of_inst != `BRANCH);
    assign write_enable = (part_of_inst != `STORE) && (part_of_inst != `BRANCH);
    assign pc_to_reg = (part_of_inst == `JAL) || (part_of_inst == `JALR);
    assign alu_op = part_of_inst;
    assign is_ecall = (part_of_inst == `ECALL);
endmodule
