`include "def_ucontroller.v"
`include "def_aluctl.v"

module ucontroller (
    input [6:0] opcode,
    input bcond,
    input reset,
    input clk,
    output pc_write,
    output pc_write_not_cond,
    output i_or_d,
    output mem_read,
    output mem_write,
    output ir_write,
    output mem_to_reg,
    output reg_write,
    output alu_src_a,
    output [1:0] alu_src_b,
    output [1:0] alu_op,
    output pc_src,
    output alu_out_write
);
    wire [`UPC_SIZE-1:0] upc_incr;
    wire [`UPC_SIZE-1:0] next_upc;
    wire [`UPC_SIZE-1:0] read_upc;
    wire [`UINSTR_SIZE-1:0] uinstr;

    asl asl_mod(
        .opcode(opcode),
        .bcond(bcond),
        .upc_incr(upc_incr),
        .seq_control(uinstr[`SEQ_CONTROL_SIZE-1:0]),
        .next_upc(next_upc)
    );

    d_reg #(.SIZE(`UPC_SIZE)) upc(
        .reset(reset),
        .data(next_upc),
        .write(1),
        .clk(clk),
        .out(read_upc)
    );

    alu #(.SIZE(`UPC_SIZE)) upc_incrementer(
        .a(read_upc),
        .b(1),
        .alu_ctl(`ALUCTL_ADD),
        .result(upc_incr)
    );

    ucode_storage ucode_storage_mod(
        .upc(read_upc),
        .uinstr(uinstr)
    );

    rt_to_control_decoder rt_to_control_decoder_mod(
        .rt_encoding(uinstr[`UINSTR_SIZE-1:`SEQ_CONTROL_SIZE]),
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
