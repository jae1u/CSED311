`include "def_opcode.v"

module cpu (
    input reset,
    input clk,
    output is_halted,
    output reg [31:0] reg_data [31:0]
);
    // control signals
    wire pc_write;
    wire pc_write_not_cond;
    wire i_or_d;
    wire mem_read;
    wire mem_write;
    wire ir_write;
    wire mem_to_reg;
    wire reg_write;
    wire alu_src_a;
    wire [1:0] alu_src_b;
    wire [1:0] alu_op;
    wire pc_src;
    wire alu_out_write;

    // inter-module wires
    wire pc_actual_write;
    wire [31:0] next_pc;
    wire [31:0] pc_val /* verilator public */;
    wire [31:0] alu_out_val;
    wire [31:0] mem_addr;
    wire [31:0] mem_read_data;
    wire [31:0] ir_val;
    wire [31:0] mdr_val;
    wire [31:0] rf_write_data;
    wire [31:0] rf_read_data_1;
    wire [31:0] rf_read_data_2;
    wire [31:0] read_data_a_val;
    wire [31:0] read_data_b_val;
    wire [31:0] imm;
    wire [3:0] alu_ctl;
    wire [31:0] alu_operand_a;
    wire [31:0] alu_operand_b;
    wire [31:0] alu_result;
    wire bcond;
    wire [31:0] x17_val;
    
    assign pc_actual_write = pc_write | (pc_write_not_cond & ~bcond);
    assign bcond = alu_result[0];
    assign is_halted = (ir_val[6:0] == `ECALL) && (x17_val == 10);

    d_reg pc(
        .reset(reset),
        .data(next_pc),
        .write(pc_actual_write),
        .clk(clk),
        .out(pc_val)
    );

    mux #(.K(32)) mem_addr_mux(
        .ins({alu_out_val, pc_val}),
        .select(i_or_d),
        .out(mem_addr)
    );

    memory memory_mod(
        .addr(mem_addr),
        ._testing_manual_reset(0),
        .write_data(read_data_b_val),
        .reset(reset),
        .mem_read(mem_read),
        .mem_write(mem_write),
        .clk(clk),
        .mem_data(mem_read_data)
    );

    d_reg ir(
        .reset(reset),
        .data(mem_read_data),
        .write(ir_write),
        .clk(clk),
        .out(ir_val)
    );

    d_reg mdr(
        .reset(reset),
        .data(mem_read_data),
        .write(1),
        .clk(clk),
        .out(mdr_val)
    );

    mux #(.K(32)) rf_write_data_mux(
        .ins({mdr_val, alu_out_val}),
        .select(mem_to_reg),
        .out(rf_write_data)
    );

    register_file rf(
        .read_reg_1(ir_val[19:15]),
        .read_reg_2(ir_val[24:20]),
        .write_reg(ir_val[11:7]),
        .write_data(rf_write_data),
        .reset(reset),
        .reg_write(reg_write),
        .clk(clk),
        ._testing_manual_reset(0),
        .read_data_1(rf_read_data_1),
        .read_data_2(rf_read_data_2),
        .x17(x17_val),
        ._data(reg_data)
    );
    
    d_reg read_data_a(
        .reset(reset),
        .data(rf_read_data_1),
        .write(1),
        .clk(clk),
        .out(read_data_a_val)
    );

    d_reg read_data_b(
        .reset(reset),
        .data(rf_read_data_2),
        .write(1),
        .clk(clk),
        .out(read_data_b_val)
    );

    imm_gen imm_gen_mod(
        .instr(ir_val),
        .imm(imm)
    );

    alu_control alu_control_mod(
        .alu_op(alu_op),
        .funct({ir_val[30], ir_val[14:12]}),
        .alu_ctl(alu_ctl)
    );

    mux #(.K(32)) alu_operand_a_mux(
        .ins({read_data_a_val, pc_val}),
        .select(alu_src_a),
        .out(alu_operand_a)
    );

    mux #(.N(2), .K(32)) alu_operand_b_mux(
        .ins({0, imm, 4, read_data_b_val}),
        .select(alu_src_b),
        .out(alu_operand_b)
    );

    alu alu_mod(
        .a(alu_operand_a),
        .b(alu_operand_b),
        .alu_ctl(alu_ctl),
        .result(alu_result)
    );

    d_reg alu_out(
        .reset(reset),
        .data(alu_result),
        .write(alu_out_write),
        .clk(clk),
        .out(alu_out_val)
    );

    mux #(.K(32)) next_pc_mux(
        .ins({alu_out_val, alu_result}),
        .select(pc_src),
        .out(next_pc)
    );

    ucontroller ucontroller_mod(
        .opcode(ir_val[6:0]),
        .bcond(bcond),
        .reset(reset),
        .clk(clk),
        .pc_write(pc_write),
        .pc_write_not_cond(pc_write_not_cond),
        .i_or_d(i_or_d),
        .mem_read(mem_read),
        .mem_write(mem_write),
        .ir_write(ir_write),
        .mem_to_reg(mem_to_reg),
        .reg_write(reg_write),
        .alu_src_a(alu_src_a),
        .alu_src_b(alu_src_b),
        .alu_op(alu_op),
        .pc_src(pc_src),
        .alu_out_write(alu_out_write)
    );
endmodule
