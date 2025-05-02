module cpu(input reset,                     // positive reset signal
           input clk,                       // clock signal
           output is_halted,                // Whehther to finish simulation
           output [31:0]print_reg[0:31]);   // Whehther to finish simulation

  /***** Wire declarations *****/
  wire [31:0] IF_pc;    // From the program counter
  wire [31:0] IF_inst;   // From the instruction memory
  wire [31:0] ID_rs1_dout;      // From the register file
  wire [31:0] ID_rs2_dout;      // From the register file
  wire ID_mem_read;             // From the control unit
  wire ID_mem_to_reg;           // From the control unit
  wire ID_mem_write;            // From the control unit
  wire ID_alu_src;              // From the control unit
  wire ID_write_enable;         // From the control unit
  wire [6:0] ID_ALUOp;          // From the control unit
  wire ID_is_ecall;             // From the control unit
  wire [31:0] ID_imm_gen_out;   // From the immediate generator
  wire [3:0] EX_alu_op;         // From the ALU control unit
  wire [31:0] EX_alu_result;    // From the ALU
  wire EX_alu_bcond;            // From the ALU
  wire [31:0] MEM_dout;          // From the data memory
  wire [1:0] ForwardA;       // From the hazard detection unit
  wire [1:0] ForwardB;       // From the hazard detection unit
  wire ID_is_jal;
  wire ID_is_jalr;
  wire ID_branch;
  wire ID_pc_to_reg;

  /***** Register declarations *****/
  reg [31:0] next_pc;
  reg is_stall;

  assign is_halted = MEM_WB_halt;
  
  wire flush = EX_PCSrc && (EX_branched_pc != IF_ID_pc);

  always @(*) begin
    if (is_stall) begin
      next_pc = IF_pc;
    end
    else begin
      next_pc = EX_PCSrc ? EX_branched_pc : (IF_pc + 4);
    end
  end

  /***** IF/ID pipeline registers *****/
  reg [31:0] IF_ID_pc;       // will be used in ID stage
  reg [31:0] IF_ID_inst;     // will be used in ID stage
  /***** ID/EX pipeline registers *****/
  // From the control unit
  reg [6:0] ID_EX_alu_op;    // will be used in EX stage
  reg ID_EX_alu_src;         // will be used in EX stage
  reg ID_EX_mem_write;       // will be used in MEM stage
  reg ID_EX_mem_read;        // will be used in MEM stage
  reg ID_EX_mem_to_reg;      // will be used in WB stage
  reg ID_EX_reg_write;       // will be used in WB stage
  reg ID_EX_is_jal;
  reg ID_EX_is_jalr;
  reg ID_EX_branch;
  reg ID_EX_pc_to_reg;
  // From others
  reg [31:0] ID_EX_pc;       // will be used in EX stage
  reg [4:0] ID_EX_rs1;
  reg [4:0] ID_EX_rs2;
  reg [4:0] ID_EX_rd;
  reg [31:0] ID_EX_rs1_data;
  reg [31:0] ID_EX_rs2_data;
  reg [31:0] ID_EX_imm;
  reg [9:0] ID_EX_ALU_ctrl_unit_input;
  reg ID_EX_halt;
  /***** EX/MEM pipeline registers *****/
  // From the control unit
  reg EX_MEM_mem_write;      // will be used in MEM stage
  reg EX_MEM_mem_read;       // will be used in MEM stage
  reg EX_MEM_mem_to_reg;     // will be used in WB stage
  reg EX_MEM_reg_write;      // will be used in WB stage
  reg EX_MEM_pc_to_reg;
  // From others
  reg [31:0] EX_MEM_pc;
  reg [31:0] EX_MEM_alu_out;
  reg [31:0] EX_MEM_dmem_data;
  reg [4:0] EX_MEM_rd;
  reg EX_MEM_halt;
  /***** MEM/WB pipeline registers *****/
  // From the control unit
  reg MEM_WB_reg_write;      // will be used in WB stage
  // From others
  reg [4:0] MEM_WB_rd;
  reg [31:0] MEM_WB_rd_din;
  reg MEM_WB_halt;

  // ---------- Update program counter ----------
  // PC must be updated on the rising edge (positive edge) of the clock.
  PC pc(
    .reset(reset),            // input (Use reset to initialize PC. Initial value must be 0)
    .clk(clk),                // input
    .next_pc(next_pc),        // input
    .current_pc(IF_pc)   // output
  );
  
  // ---------- Instruction Memory ----------
  InstMemory imem(
    .reset(reset),       // input
    .clk(clk),           // input
    .addr(IF_pc),   // input
    .dout(IF_inst)   // output
  );

  // Update IF/ID pipeline registers here
  always @(posedge clk) begin
    if (reset || flush) begin
      IF_ID_inst <= 0;
      IF_ID_pc <= 0;
    end
    else if (!is_stall) begin
      IF_ID_inst <= IF_inst;   // will be used in ID stage
      IF_ID_pc <= IF_pc;      // will be used in ID stage
    end
  end
  wire [4:0] IF_ID_rs1 = (IF_ID_inst[6:0] == 7'b1110011) ? 17 : IF_ID_inst[19:15];
  wire [4:0] IF_ID_rs2 = IF_ID_inst[24:20];

  // ---------- Register File ----------
  RegisterFile reg_file (
    .reset (reset),                     // input
    .clk (clk),                         // input
    .rs1 (IF_ID_rs1),                   // input
    .rs2 (IF_ID_rs2),                   // input
    .rd (MEM_WB_rd),                    // input
    .rd_din (MEM_WB_rd_din),            // input
    .write_enable (MEM_WB_reg_write),   // input
    .rs1_dout (ID_rs1_dout),               // output
    .rs2_dout (ID_rs2_dout),               // output
    .print_reg(print_reg)
  );

  // ---------- Control Unit ----------
  ControlUnit ctrl_unit (
    .part_of_inst(IF_ID_inst[6:0]),   // input
    .is_jal(ID_is_jal),                  // output
    .is_jalr(ID_is_jalr),                // output
    .branch(ID_branch),                  // output
    .mem_read(ID_mem_read),              // output
    .mem_to_reg(ID_mem_to_reg),          // output
    .mem_write(ID_mem_write),            // output
    .alu_src(ID_alu_src),                // output
    .write_enable(ID_write_enable),      // output
    .pc_to_reg(ID_pc_to_reg),            // output
    .alu_op(ID_ALUOp),                   // output
    .is_ecall(ID_is_ecall)               // output (ecall inst)
  );

  // ---------- Immediate Generator ----------
  ImmediateGenerator imm_gen(
    .part_of_inst(IF_ID_inst),   // input
    .imm_gen_out(ID_imm_gen_out)    // output
  );

  // Update ID/EX pipeline registers here
  always @(posedge clk) begin
    if (reset || is_stall || flush) begin
      // From the control unit
      ID_EX_alu_op <= 0;       // will be used in EX stage
      ID_EX_alu_src <= 0;      // will be used in EX stage
      ID_EX_mem_write <= 0;    // will be used in MEM stage
      ID_EX_mem_read <= 0;     // will be used in MEM stage
      ID_EX_mem_to_reg <= 0;   // will be used in WB stage
      ID_EX_reg_write <= 0;    // will be used in WB stage
      ID_EX_is_jal <= 0;
      ID_EX_is_jalr <= 0;
      ID_EX_branch <= 0;
      ID_EX_pc_to_reg <= 0;
      // From others
      ID_EX_pc <= 0;           // will be used in EX stage
      ID_EX_rs1_data <= 0;
      ID_EX_rs2_data <= 0;
      ID_EX_imm <= 0;
      ID_EX_ALU_ctrl_unit_input <= 0;
      ID_EX_rd <= 0;
      ID_EX_halt <= 0;
      ID_EX_rs1 <= 0;
      ID_EX_rs2 <= 0;
    end
    else begin
      // From the control unit
      ID_EX_alu_op <= ID_ALUOp;             // will be used in EX stage
      ID_EX_alu_src <= ID_alu_src;          // will be used in EX stage
      ID_EX_mem_write <= ID_mem_write;      // will be used in MEM stage
      ID_EX_mem_read <= ID_mem_read;        // will be used in MEM stage
      ID_EX_mem_to_reg <= ID_mem_to_reg;    // will be used in WB stage
      ID_EX_reg_write <= ID_write_enable;   // will be used in WB stage
      ID_EX_is_jal <= ID_is_jal;
      ID_EX_is_jalr <= ID_is_jalr;
      ID_EX_branch <= ID_branch;
      ID_EX_pc_to_reg <= ID_pc_to_reg;
      // From others
      ID_EX_pc <= IF_ID_pc;              // will be used in EX stage
      ID_EX_rs1_data <= ID_rs1_dout;
      ID_EX_rs2_data <= ID_rs2_dout;
      ID_EX_imm <= ID_imm_gen_out;
      ID_EX_ALU_ctrl_unit_input <= {IF_ID_inst[31:25], IF_ID_inst[14:12]};
      ID_EX_rd <= IF_ID_inst[11:7];
      ID_EX_halt <= ID_is_ecall && (((EX_MEM_rd == 17 && EX_MEM_reg_write) ? EX_MEM_alu_out : ID_rs1_dout) == 10);
      ID_EX_rs1 <= IF_ID_rs1;
      ID_EX_rs2 <= IF_ID_rs2;
    end
  end

  // ---------- ALU Control Unit ----------
  ALUControlUnit alu_ctrl_unit (
    .part_of_inst({ID_EX_ALU_ctrl_unit_input, ID_EX_alu_op}),   // input
    .alu_op(EX_alu_op)                                             // output
  );

  // ---------- ALU ----------
  ALU alu (
    .alu_op(EX_alu_op),                                                                                                              // input
    .alu_in_1(ForwardA == 0 ? ID_EX_rs1_data : (ForwardA == 1 ? EX_MEM_alu_out : MEM_WB_rd_din)),                                 // input  
    .alu_in_2(ID_EX_alu_src ? ID_EX_imm : (ForwardB == 0 ? ID_EX_rs2_data : (ForwardB == 1 ? EX_MEM_alu_out : MEM_WB_rd_din))),   // input
    .alu_result(EX_alu_result),                                                                                                      // output
    .alu_bcond(EX_alu_bcond)                                                                                                         // output
  );
  wire EX_PCSrc = ID_EX_is_jal || ID_EX_is_jalr || (ID_EX_branch && EX_alu_bcond);
  wire [31:0] EX_branched_pc = ID_EX_is_jalr ? EX_alu_result : (ID_EX_pc + ID_EX_imm);

  // Update EX/MEM pipeline registers here
  always @(posedge clk) begin
    if (reset) begin
      // From the control unit
      EX_MEM_mem_write <= 0;      // will be used in MEM stage
      EX_MEM_mem_read <= 0;       // will be used in MEM stage
      EX_MEM_mem_to_reg <= 0;     // will be used in WB stage
      EX_MEM_reg_write <= 0;      // will be used in WB stage
      EX_MEM_pc_to_reg <= 0;
      // From others
      EX_MEM_pc <= 0;
      EX_MEM_alu_out <= 0;
      EX_MEM_dmem_data <= 0;
      EX_MEM_rd <= 0;
      EX_MEM_halt <= 0;
    end
    else begin
      // From the control unit
      EX_MEM_mem_write <= ID_EX_mem_write;     // will be used in MEM stage
      EX_MEM_mem_read <= ID_EX_mem_read;       // will be used in MEM stage
      EX_MEM_mem_to_reg <= ID_EX_mem_to_reg;   // will be used in WB stage
      EX_MEM_reg_write <= ID_EX_reg_write;     // will be used in WB stage
      EX_MEM_pc_to_reg <= ID_EX_pc_to_reg;
      // From others
      EX_MEM_pc <= ID_EX_pc;
      EX_MEM_alu_out <= EX_alu_result;
      EX_MEM_dmem_data <= ForwardB == 0 ? ID_EX_rs2_data : (ForwardB == 1 ? EX_MEM_alu_out : MEM_WB_rd_din);
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
    .dout (MEM_dout)                     // output
  );

  // Update MEM/WB pipeline registers here
  always @(posedge clk) begin
    if (reset) begin
      // From the control unit
      MEM_WB_reg_write <= 0;    // will be used in WB stage
      // From others
      MEM_WB_rd_din <= 0;
      MEM_WB_halt <= 0;
      MEM_WB_rd <= 0;
    end
    else begin
      // From the control unit
      MEM_WB_reg_write <= EX_MEM_reg_write;     // will be used in WB stage
      // From others
      MEM_WB_rd_din <= EX_MEM_pc_to_reg ? (EX_MEM_pc + 4): (EX_MEM_mem_to_reg ? MEM_dout : EX_MEM_alu_out);
      MEM_WB_halt <= EX_MEM_halt;
      MEM_WB_rd <= EX_MEM_rd;
    end
  end

  ForwardingUnit forwarding_unit(
    .rs1_ex(ID_EX_rs1),
    .rs2_ex(ID_EX_rs2),
    .rd_mem(EX_MEM_rd),
    .rd_wb(MEM_WB_rd),
    .reg_write_mem(EX_MEM_reg_write),
    .reg_write_wb(MEM_WB_reg_write),
    .ForwardA(ForwardA),
    .ForwardB(ForwardB)
  );

  HazardDetectionUnit hazard_detection_unit(
    .rs1_id(IF_ID_rs1),
    .rs2_id(IF_ID_rs2),
    .rd_ex(ID_EX_rd),
    .reg_write_ex(ID_EX_reg_write),
    .mem_read_ex(ID_EX_mem_read),
    .is_ecall(ID_is_ecall),
    .is_stall(is_stall)
  );

endmodule
