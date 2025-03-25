`include "def_ucontroller.v"

module ucode_storage (
    input [`UPC_SIZE-1:0] upc,
    output reg [`UINSTR_SIZE-1:0] uinstr
);
    always @(*) begin
        case (upc)
            `UINSTR_PC_IF: uinstr = `UINSTR_IF;
            `UINSTR_PC_ID: uinstr = `UINSTR_ID;
            `UINSTR_PC_REX: uinstr = `UINSTR_REX;
            `UINSTR_PC_RWB: uinstr = `UINSTR_RWB;
            `UINSTR_PC_MEM_ADDR: uinstr = `UINSTR_MEM_ADDR;
            `UINSTR_PC_LD_MEM_READ: uinstr = `UINSTR_LD_MEM_READ;
            `UINSTR_PC_LD_WB: uinstr = `UINSTR_LD_WB;
            `UINSTR_PC_SD_MEM_WRITE: uinstr = `UINSTR_SD_MEM_WRITE;
            `UINSTR_PC_B_CHECK: uinstr = `UINSTR_B_CHECK;
            `UINSTR_PC_BRANCH: uinstr = `UINSTR_BRANCH;
            `UINSTR_PC_JAL: uinstr = `UINSTR_JAL;
            `UINSTR_PC_JALR: uinstr = `UINSTR_JALR;
            `UINSTR_PC_IEX: uinstr = `UINSTR_IEX;
            `UINSTR_PC_NOP: uinstr = `UINSTR_NOP;
            default: uinstr = `UINSTR_IF;
        endcase
    end
    
endmodule
