module ForwardingUnit (
    input [4:0] rs1_ex,
    input [4:0] rs2_ex,
    input [4:0] rd_mem,
    input [4:0] rd_wb,
    input reg_write_mem,
    input reg_write_wb,
    output reg [1:0] ForwardA,
    output reg [1:0] ForwardB);

    always @(*) begin
        if (rs1_ex != 0 && rs1_ex == rd_mem && reg_write_mem) begin
            ForwardA = 1;
        end else if (rs1_ex != 0 && rs1_ex == rd_wb && reg_write_wb) begin
            ForwardA = 2;
        end else begin
            ForwardA = 0;
        end
        if (rs2_ex != 0 && rs2_ex == rd_mem && reg_write_mem) begin
            ForwardB = 1;
        end else if (rs2_ex != 0 && rs2_ex == rd_wb && reg_write_wb) begin
            ForwardB = 2;
        end else begin
            ForwardB = 0;
        end
    end

endmodule
