`include "def_ucontroller.v"
`include "def_opcode.v"

module asl (
    input [6:0] opcode,
    input bcond,
    input [`UPC_SIZE-1:0] upc_incr,
    input [`SEQ_CONTROL_SIZE-1:0] seq_control,
    output reg [`UPC_SIZE-1:0] next_upc
);
    always @(*) begin
        case (seq_control)
            `SEQ_CONTROL_RESET: next_upc = `UINSTR_PC_IF;
            `SEQ_CONTROL_DISPATCH_ID: begin
                case (opcode)
                    `ARITHMETIC: next_upc = `UINSTR_PC_REX;
                    `ARITHMETIC_IMM: next_upc = `UINSTR_PC_IEX;
                    `LOAD: next_upc = `UINSTR_PC_MEM_ADDR;
                    `STORE: next_upc = `UINSTR_PC_MEM_ADDR;
                    `BRANCH: next_upc = `UINSTR_PC_B_CHECK;
                    `JAL: next_upc = `UINSTR_PC_JAL;
                    `JALR: next_upc = `UINSTR_PC_JALR;
                    `ECALL: next_upc = `UINSTR_PC_NOP;
                    default: next_upc = `UINSTR_PC_IF;
                endcase
            end
            `SEQ_CONTROL_DISPATCH_MEM_ADDR: begin
                case (opcode)
                    `LOAD: next_upc = `UINSTR_PC_LD_MEM_READ;
                    `STORE: next_upc = `UINSTR_PC_SD_MEM_WRITE;
                    default: next_upc = `UINSTR_PC_IF;
                endcase
            end
            `SEQ_CONTROL_DISPATCH_B_CHECK: begin
                if (bcond) next_upc = `UINSTR_PC_BRANCH;
                else next_upc = `UINSTR_PC_IF;
            end
            `SEQ_CONTROL_DISPATCH_IEX: next_upc = `UINSTR_PC_RWB;
            `SEQ_CONTROL_INCREMENT: next_upc = upc_incr;
            default: next_upc = `UINSTR_PC_IF;
        endcase      
    end
endmodule
