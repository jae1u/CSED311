// Submit this file with other files you created.
// Do not touch port declarations of the module 'CPU'.

// Guidelines
// 1. It is highly recommened to `define opcodes and something useful.
// 2. You can modify modules (except InstMemory, DataMemory, and RegisterFile)
// (e.g., port declarations, remove modules, define new modules, ...)
// 3. You might need to describe combinational logics to drive them into the module (e.g., mux, and, or, ...)
// 4. `include files if required

module cpu(input reset,                     // positive reset signal
           input clk,                       // clock signal
           output is_halted,                // Whehther to finish simulation
           output [31:0]print_reg[0:31]);   // Whehther to finish simulation

  /***** Wire declarations *****/
  wire [31:0] current_pc;
  wire [31:0] instruction;
  wire [31:0] rs1_dout;
  wire [31:0] rs2_dout;
  wire mem_read;
  wire mem_to_reg;
  wire mem_write;
  wire alu_src;
  wire write_enable;
  // wire pc_to_reg;       (TODO)
  wire [6:0] ALUOp;
  wire is_ecall;
  wire [31:0] imm_gen_out;
  wire [3:0] alu_op;
  wire [31:0] alu_result;
  wire alu_zero;
  wire [31:0] dout;
  // wire [31:0] target;   (TODO)
  // wire PCSrc1;          (TODO)
  // wire is_jal;          (TODO)
  // wire is_jalr;         (TODO)
  // wire branch;          (TODO)

  /***** Register declarations *****/
  reg [31:0] next_pc;
  reg is_stall;
  reg ID_EX_halt;
  reg EX_MEM_halt;
  reg MEM_WB_halt;
  reg [4:0] MEM_WB_rd;

  assign is_halted = MEM_WB_halt;
  always @(*) begin
    if (is_stall) begin
      next_pc = current_pc;
    end
    else begin
      next_pc = current_pc + 4;
    end
  end
  // assign target = current_pc + imm_gen_out;                                       (TODO)
  // assign PCSrc1 = is_jal || (branch && alu_bcond);                                (TODO)
  // assign next_pc = is_jalr ? alu_result : (PCSrc1 ? target : (current_pc + 4));   (TODO)

  // You need to modify the width of registers
  // In addition, 
  // 1. You might need other pipeline registers that are not described below
  // 2. You might not need registers described below
  /***** IF/ID pipeline registers *****/
  reg [31:0] IF_ID_inst;    // will be used in ID stage
  /***** ID/EX pipeline registers *****/
  // From the control unit
  reg [6:0] ID_EX_alu_op;   // will be used in EX stage
  reg ID_EX_alu_src;        // will be used in EX stage
  reg ID_EX_mem_write;      // will be used in MEM stage
  reg ID_EX_mem_read;       // will be used in MEM stage
  reg ID_EX_mem_to_reg;     // will be used in WB stage
  reg ID_EX_reg_write;      // will be used in WB stage
  // From others
  reg [31:0] ID_EX_rs1_data;
  reg [31:0] ID_EX_rs2_data;
  reg [31:0] ID_EX_imm;
  reg [9:0] ID_EX_ALU_ctrl_unit_input;
  reg [4:0] ID_EX_rd;
  /***** EX/MEM pipeline registers *****/
  // From the control unit
  reg EX_MEM_mem_write;     // will be used in MEM stage
  reg EX_MEM_mem_read;      // will be used in MEM stage
  // reg EX_MEM_is_branch;  // will be used in MEM stage
  reg EX_MEM_mem_to_reg;    // will be used in WB stage
  reg EX_MEM_reg_write;     // will be used in WB stage
  // From others
  reg [31:0] EX_MEM_alu_out;
  reg [31:0] EX_MEM_dmem_data;
  reg [4:0] EX_MEM_rd;
  /***** MEM/WB pipeline registers *****/
  // From the control unit
  reg MEM_WB_mem_to_reg;    // will be used in WB stage
  reg MEM_WB_reg_write;     // will be used in WB stage
  // From others
  reg [31:0] MEM_WB_mem_to_reg_src_1;
  reg [31:0] MEM_WB_mem_to_reg_src_2;

  // ---------- Update program counter ----------
  // PC must be updated on the rising edge (positive edge) of the clock.
  PC pc(
    .reset(reset),            // input (Use reset to initialize PC. Initial value must be 0)
    .clk(clk),                // input
    .next_pc(next_pc),        // input
    .current_pc(current_pc)   // output
  );
  
  // ---------- Instruction Memory ----------
  InstMemory imem(
    .reset(reset),         // input
    .clk(clk),             // input
    .addr(current_pc),     // input
    .dout(instruction)     // output
  );

  // Update IF/ID pipeline registers here
  always @(posedge clk) begin
    if (reset) begin
      IF_ID_inst <= 0;
    end
    else if (!is_stall) begin
      IF_ID_inst <= instruction;   // will be used in ID stage
    end
  end

  // ---------- Register File ----------
  RegisterFile reg_file (
    .reset (reset),                                                                    // input
    .clk (clk),                                                                        // input
    .rs1 (is_ecall ? 17 : IF_ID_inst[19:15]),                                          // input
    .rs2 (IF_ID_inst[24:20]),                                                          // input
    .rd (MEM_WB_rd),                                                                   // input
    // .rd_din (pc_to_reg ? next_pc : (mem_to_reg ? mem_data : alu_result)),           // input (TODO)
    .rd_din (MEM_WB_mem_to_reg ? MEM_WB_mem_to_reg_src_1 : MEM_WB_mem_to_reg_src_2),   // input
    .write_enable (MEM_WB_reg_write),                                                  // input
    .rs1_dout (rs1_dout),                                                              // output
    .rs2_dout (rs2_dout),                                                              // output
    .print_reg(print_reg)
  );

  // ---------- Control Unit ----------
  ControlUnit ctrl_unit (
    .part_of_inst(IF_ID_inst[6:0]),   // input
    .mem_read(mem_read),              // output
    .mem_to_reg(mem_to_reg),          // output
    .mem_write(mem_write),            // output
    .alu_src(alu_src),                // output
    .write_enable(write_enable),      // output
    // .pc_to_reg(pc_to_reg),         // output
    .alu_op(ALUOp),                   // output
    .is_ecall(is_ecall)               // output (ecall inst)
  );

  // ---------- Immediate Generator ----------
  ImmediateGenerator imm_gen(
    .part_of_inst(IF_ID_inst),   // input
    .imm_gen_out(imm_gen_out)    // output
  );

  // Update ID/EX pipeline registers here
  always @(posedge clk) begin
    if (reset || is_stall) begin
      // From the control unit
      ID_EX_alu_op <= 0;       // will be used in EX stage
      ID_EX_alu_src <= 0;      // will be used in EX stage
      ID_EX_mem_write <= 0;    // will be used in MEM stage
      ID_EX_mem_read <= 0;     // will be used in MEM stage
      ID_EX_mem_to_reg <= 0;   // will be used in WB stage
      ID_EX_reg_write <= 0;    // will be used in WB stage
      // From others
      ID_EX_rs1_data <= 0;
      ID_EX_rs2_data <= 0;
      ID_EX_imm <= 0;
      ID_EX_ALU_ctrl_unit_input <= 0;
      ID_EX_rd <= 0;
      ID_EX_halt <= 0;
    end
    else begin
      // From the control unit
      ID_EX_alu_op <= ALUOp;             // will be used in EX stage
      ID_EX_alu_src <= alu_src;          // will be used in EX stage
      ID_EX_mem_write <= mem_write;      // will be used in MEM stage
      ID_EX_mem_read <= mem_read;        // will be used in MEM stage
      ID_EX_mem_to_reg <= mem_to_reg;    // will be used in WB stage
      ID_EX_reg_write <= write_enable;   // will be used in WB stage
      // From others
      ID_EX_rs1_data <= rs1_dout;
      ID_EX_rs2_data <= rs2_dout;
      ID_EX_imm <= imm_gen_out;
      ID_EX_ALU_ctrl_unit_input <= {IF_ID_inst[31:25], IF_ID_inst[14:12]};
      ID_EX_rd <= IF_ID_inst[11:7];
      ID_EX_halt <= is_ecall && (rs1_dout == 10);
    end
  end

  // ---------- ALU Control Unit ----------
  ALUControlUnit alu_ctrl_unit (
    .part_of_inst({ID_EX_ALU_ctrl_unit_input, ID_EX_alu_op}),   // input
    .alu_op(alu_op)                                             // output
  );

  // ---------- ALU ----------
  ALU alu (
    .alu_op(alu_op),                                         // input
    .alu_in_1(ID_EX_rs1_data),                               // input  
    .alu_in_2(ID_EX_alu_src ? ID_EX_imm : ID_EX_rs2_data),   // input
    .alu_result(alu_result),                                 // output
    .alu_zero(alu_zero)                                      // output
  );

  // Update EX/MEM pipeline registers here
  always @(posedge clk) begin
    if (reset) begin
      // From the control unit
      EX_MEM_mem_write <= 0;      // will be used in MEM stage
      EX_MEM_mem_read <= 0;       // will be used in MEM stage
      // EX_MEM_is_branch <= 0;   // will be used in MEM stage
      EX_MEM_mem_to_reg <= 0;     // will be used in WB stage
      EX_MEM_reg_write <= 0;      // will be used in WB stage
      // From others
      EX_MEM_alu_out <= 0;
      EX_MEM_dmem_data <= 0;
      EX_MEM_rd <= 0;
      EX_MEM_halt <= 0;
    end
    else begin
      // From the control unit
      EX_MEM_mem_write <= ID_EX_mem_write;     // will be used in MEM stage
      EX_MEM_mem_read <= ID_EX_mem_read;       // will be used in MEM stage
      // EX_MEM_is_branch <= alu_zero;         // will be used in MEM stage
      EX_MEM_mem_to_reg <= ID_EX_mem_to_reg;   // will be used in WB stage
      EX_MEM_reg_write <= ID_EX_reg_write;     // will be used in WB stage
      // From others
      EX_MEM_alu_out <= alu_result;
      EX_MEM_dmem_data <= ID_EX_rs2_data;
      EX_MEM_rd <= ID_EX_rd;
      EX_MEM_halt <= ID_EX_halt;
    end
  end

  // ---------- Data Memory ----------
  DataMemory dmem(
    .reset (reset),                  // input
    .clk (clk),                      // input
    .addr (EX_MEM_alu_out),          // input
    .din (EX_MEM_dmem_data),         // input
    .mem_read (EX_MEM_mem_read),     // input
    .mem_write (EX_MEM_mem_write),   // input
    .dout (dout)                     // output
  );

  // Update MEM/WB pipeline registers here
  always @(posedge clk) begin
    if (reset) begin
      // From the control unit
      MEM_WB_mem_to_reg <= 0;   // will be used in WB stage
      MEM_WB_reg_write <= 0;    // will be used in WB stage
      // From others
      MEM_WB_mem_to_reg_src_1 <= 0;
      MEM_WB_mem_to_reg_src_2 <= 0;
      MEM_WB_halt <= 0;
      MEM_WB_rd <= 0;
    end
    else begin
      // From the control unit
      MEM_WB_mem_to_reg <= EX_MEM_mem_to_reg;   // will be used in WB stage
      MEM_WB_reg_write <= EX_MEM_reg_write;     // will be used in WB stage
      // From others
      MEM_WB_mem_to_reg_src_1 <= dout;
      MEM_WB_mem_to_reg_src_2 <= EX_MEM_alu_out;
      MEM_WB_halt <= EX_MEM_halt;
      MEM_WB_rd <= EX_MEM_rd;
    end
  end

  HazardDetectionUnit hazard_detection_unit(
    .rs1_id(is_ecall ? 17 : IF_ID_inst[19:15]),
    .rs2_id(IF_ID_inst[24:20]),
    .rd_ex(ID_EX_rd),
    .rd_mem(EX_MEM_rd),
    .reg_write_ex(ID_EX_reg_write),
    .reg_write_mem(EX_MEM_reg_write),
    .is_stall(is_stall)
  );

endmodule
