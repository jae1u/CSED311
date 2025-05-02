module HazardDetectionUnit(
    input [4:0] rs1_id,
    input [4:0] rs2_id,
    input [4:0] rd_ex,
    input reg_write_ex,
    input mem_read_ex,
    input is_ecall,
    output is_stall);

    // TODO: Maybe we neet to check if instruction is really use rs1 or rs2
    wire use_rs1 = (rs1_id != 0);
    wire use_rs2 = (rs2_id != 0);

    wire is_forwarding_stall = ((rs1_id == rd_ex) && use_rs1 || (rs2_id == rd_ex) && use_rs2) && mem_read_ex;
    wire is_ecall_stall = is_ecall && rd_ex == 17 && reg_write_ex;
    assign is_stall = is_forwarding_stall || is_ecall_stall;

endmodule
