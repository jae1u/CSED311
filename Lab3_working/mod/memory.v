/* 2^N x 4 byte memory*/
module memory #(
    parameter N = 15
) (
    /* verilator lint_off UNUSEDSIGNAL */
    input [31:0] addr,
    input _testing_manual_reset /* verilator public */,
    /* verilator lint_on UNUSEDSIGNAL */
    input [31:0] write_data,
    input reset,
    input mem_read,
    input mem_write,
    input clk,
    output reg [31:0] mem_data
);

    wire [N - 1:0] idx;
    reg [31:0] mem [2**N - 1:0] /* verilator public */;

    assign idx = {{(2){1'b0}}, addr[N - 1:2]};

    always @(*) begin
        if (mem_read) mem_data = mem[idx];
        else mem_data = 32'b0;
    end

    always @(posedge clk) begin
        if (reset && !_testing_manual_reset) begin
            for (integer i = 0; i < 2**N; i = i + 1)
                mem[i] <= 32'b0; 
        end
        else begin
            if (mem_write) begin
                mem[idx] <= write_data;
            end
        end
    end
    
endmodule
