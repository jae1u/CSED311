module register_file(input	reset,
                     input clk,
                     input [4:0] rs1,          // source register 1
                     input [4:0] rs2,          // source register 2
                     input [4:0] rd,           // destination register
                     input [31:0] rd_din,      // input data for rd
                     input write_enable,          // RegWrite signal
                     output [31:0] rs1_dout,   // output of rs 1
                     output [31:0] rs2_dout,   // output of rs 2
                     output [31:0] print_reg [0:31],
                     output [31:0] x17);
  integer i;
  // Register file
  reg [31:0] rf[0:31];
  // Do not touch or use print_reg
  assign print_reg = rf;

  // Asynchronously read register file
  assign rs1_dout = (rs1 == 0) ? 0 : rf[rs1];
  assign rs2_dout = (rs2 == 0) ? 0 : rf[rs2];
  assign x17 = rf[17];

  // Synchronously write data to the register file
  always @(posedge clk) begin
    if (write_enable) begin
      rf[rd] <= rd_din;
    end
  end

  // Initialize register file (do not touch)
  always @(posedge clk) begin
    // Reset register file
    if (reset) begin
      for (i = 0; i < 32; i = i + 1)
        // DO NOT TOUCH COMMENT BELOW
        /* verilator lint_off BLKSEQ */
        rf[i] = 32'b0;
        /* verilator lint_on BLKSEQ */
        // DO NOT TOUCH COMMENT ABOVE

      // DO NOT TOUCH COMMENT BELOW
      /* verilator lint_off BLKSEQ */
      rf[2] = 32'h2ffc; // stack pointer
      /* verilator lint_on BLKSEQ */
      // DO NOT TOUCH COMMENT ABOVE
    end
  end
endmodule
