module register_file (
    input [4:0] read_reg_1,
    input [4:0] read_reg_2,
    input [4:0] write_reg,
    input [31:0] write_data,
    input reset,
    input reg_write,
    input clk,
    /* verilator lint_off UNUSEDSIGNAL */
    input _testing_manual_reset /* verilator public */,
    /* verilator lint_on UNUSEDSIGNAL */
    output reg [31:0] read_data_1,
    output reg [31:0] read_data_2,
    output reg [31:0] x17,
    output reg [31:0] _data [0:31]
);
    reg [31:0] r [0:31] /* verilator public */;
    
    always @(*) begin
        if (read_reg_1 == 5'b0) read_data_1 = 0;
        else read_data_1 = r[read_reg_1];

        if (read_reg_2 == 5'b0) read_data_2 = 0;
        else read_data_2 = r[read_reg_2];

        _data = r;
        x17 = r[17];
    end
    
    always @(posedge clk) begin
        if (reset && !_testing_manual_reset) begin
            for (integer i = 0; i < 32; i = i + 1)
                r[i] <= 0;
            r[2] <= 32'h2ffc;
        end
        else begin
            if (reg_write && write_reg != 5'b0) begin
                r[write_reg] <= write_data;
            end 
        end
    end
endmodule
