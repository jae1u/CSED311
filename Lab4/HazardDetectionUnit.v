module HazardDetectionUnit(
    input [4:0] rs1_id,
    input [4:0] rs2_id,
    input [4:0] rd_ex,
    input mem_read_ex,
    output reg is_stall);

    // TODO: Maybe we neet to check if instruction is really use rs1 or rs2
    wire use_rs1 = (rs1_id != 0);
    wire use_rs2 = (rs2_id != 0);

    always @(*) begin
        is_stall = ((rs1_id == rd_ex) && use_rs1 || (rs2_id == rd_ex) && use_rs2) && mem_read_ex;
    end

endmodule
