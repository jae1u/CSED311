module HazardDetectionUnit(
    input [4:0] rs1_id,
    input [4:0] rs2_id,
    input [4:0] rd_ex,
    input [4:0] rd_mem,
    input reg_write_ex,
    input reg_write_mem,
    output reg is_stall);

    wire use_rs1 = (rs1_id != 0);
    wire use_rs2 = (rs2_id != 0);

    always @(*) begin
        is_stall = rs1_id == rd_ex && use_rs1 && reg_write_ex ||
                   rs2_id == rd_ex && use_rs2 && reg_write_ex ||
                   rs1_id == rd_mem && use_rs1 && reg_write_mem ||
                   rs2_id == rd_mem && use_rs2 && reg_write_mem;
    end

endmodule
