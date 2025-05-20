module HazardDetectionUnit(
    input [4:0] rs1_id,
    input [4:0] rs2_id,
    input [4:0] rd_ex,
    input [4:0] rd_mem,
    input reg_write_ex,
    input mem_read_ex,
    input mem_read_mem,
    input is_ecall,
    input is_branch,
    output is_stall);

    // TODO: Maybe we neet to check if instruction is really use rs1 or rs2
    wire use_rs1 = (rs1_id != 0);
    wire use_rs2 = (rs2_id != 0);

    wire is_forwarding_stall = ((rs1_id == rd_ex) && use_rs1 || (rs2_id == rd_ex) && use_rs2) && mem_read_ex;
    wire id_stage_stall = (is_ecall || is_branch) && (((rs1_id == rd_ex || rs2_id == rd_ex) && reg_write_ex) || ((rs1_id == rd_mem || rs2_id == rd_mem) && mem_read_mem));
    assign is_stall = is_forwarding_stall || id_stage_stall;

endmodule
