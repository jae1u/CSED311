`include "def_ucontroller.v"
`include "def_control_signal.v"

module rt_to_control_decoder (
    input [`RT_ENCODING_SIZE-1:0] rt_encoding,
    output reg pc_write,
    output reg pc_write_not_cond,
    output reg i_or_d,
    output reg mem_read,
    output reg mem_write,
    output reg ir_write,
    output reg mem_to_reg,
    output reg reg_write,
    output reg alu_src_a,
    output reg [1:0] alu_src_b,
    output reg [1:0] alu_op,
    output reg pc_src,
    output reg alu_out_write
);

    always @(*) begin
        pc_write = 0;
        pc_write_not_cond = 0;
        i_or_d = `IORD_INST;
        mem_read = 0;
        mem_write = 0;
        ir_write = 0;
        mem_to_reg = `MEM_TO_REG_ALUOUT;
        reg_write = 0;
        alu_src_a = `ALU_SRC_A_A;
        alu_src_b = `ALU_SRC_B_B;
        alu_op = `ALUOP_RTYPE;
        pc_src = `PC_SRC_ALU_OUT;
        alu_out_write = 0;
        
        if (rt_encoding[0]) begin
            mem_read = 1;
            ir_write = 1;
        end

        if (rt_encoding[1]) begin
        end

        if (rt_encoding[2]) begin
        end

        if (rt_encoding[3]) begin
            alu_src_a = `ALU_SRC_A_PC;
            alu_src_b = `ALU_SRC_B_4;
            alu_op = `ALUOP_ADD;
            alu_out_write = 1;
        end

        if (rt_encoding[4]) begin
            alu_out_write = 1;
        end

        if (rt_encoding[5]) begin
            reg_write = 1;
        end

        if (rt_encoding[6]) begin
            pc_write = 1;
            alu_src_a = `ALU_SRC_A_PC;
            alu_src_b = `ALU_SRC_B_4;
            alu_op = `ALUOP_ADD;
            pc_src = `PC_SRC_ALU;
        end

        if (rt_encoding[7]) begin
            alu_src_b = `ALU_SRC_B_IMM;
            alu_op = `ALUOP_ADD;
            alu_out_write = 1;
        end

        if (rt_encoding[8]) begin
            i_or_d = `IORD_DATA;
            mem_read = 1;
        end

        if (rt_encoding[9]) begin
            mem_to_reg = `MEM_TO_REG_MDR;
            reg_write = 1;
        end

        if (rt_encoding[10]) begin
            i_or_d = `IORD_DATA;
            mem_write = 1;
        end

        if (rt_encoding[11]) begin
            pc_write_not_cond = 1;
            alu_op = `ALUOP_B;
        end

        if (rt_encoding[12]) begin
            pc_write = 1;
            alu_src_a = `ALU_SRC_A_PC;
            alu_src_b = `ALU_SRC_B_IMM;
            alu_op = `ALUOP_ADD;
            pc_src = `PC_SRC_ALU;
        end

        if (rt_encoding[13]) begin
            pc_write = 1;
            alu_src_b = `ALU_SRC_B_IMM;
            alu_op = `ALUOP_ADD;
            pc_src = `PC_SRC_ALU;
        end

        if (rt_encoding[14]) begin
            alu_src_b = `ALU_SRC_B_IMM;
            alu_op = `ALUOP_ITYPE;
            alu_out_write = 1;
        end
    end
    
endmodule
