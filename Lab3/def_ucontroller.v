// SeqControl
`define SEQ_CONTROL_SIZE                3
`define SEQ_CONTROL_RESET               3'b000
`define SEQ_CONTROL_DISPATCH_ID         3'b001
`define SEQ_CONTROL_DISPATCH_MEM_ADDR   3'b010
`define SEQ_CONTROL_DISPATCH_B_CHECK    3'b011
`define SEQ_CONTROL_DISPATCH_IEX        3'b100
`define SEQ_CONTROL_INCREMENT           3'b101

// RTs
`define RT_ENCODING_SIZE        15
`define RT_0                    15'b0000000000000001
`define RT_1                    15'b0000000000000010
`define RT_2                    15'b0000000000000100
`define RT_3                    15'b0000000000001000
`define RT_4                    15'b0000000000010000
`define RT_5                    15'b0000000000100000
`define RT_6                    15'b0000000001000000
`define RT_7                    15'b0000000010000000
`define RT_8                    15'b0000000100000000
`define RT_9                    15'b0000001000000000
`define RT_10                   15'b0000010000000000
`define RT_11                   15'b0000100000000000
`define RT_12                   15'b0001000000000000
`define RT_13                   15'b0010000000000000
`define RT_14                   15'b0100000000000000

// u-instructions
`define UINSTR_SIZE             `SEQ_CONTROL_SIZE + `RT_ENCODING_SIZE
`define UINSTR_IF               {`RT_0,                 `SEQ_CONTROL_INCREMENT}
`define UINSTR_ID               {`RT_1 | `RT_2 | `RT_3, `SEQ_CONTROL_DISPATCH_ID}
`define UINSTR_REX              {`RT_4,                 `SEQ_CONTROL_INCREMENT}
`define UINSTR_RWB              {`RT_5 | `RT_6,         `SEQ_CONTROL_RESET}
`define UINSTR_MEM_ADDR         {`RT_7,                 `SEQ_CONTROL_DISPATCH_MEM_ADDR}
`define UINSTR_LD_MEM_READ      {`RT_8,                 `SEQ_CONTROL_INCREMENT}
`define UINSTR_LD_WB            {`RT_6 | `RT_9,         `SEQ_CONTROL_RESET}
`define UINSTR_SD_MEM_WRITE     {`RT_6 | `RT_10,        `SEQ_CONTROL_RESET}
`define UINSTR_B_CHECK          {`RT_11,                `SEQ_CONTROL_DISPATCH_B_CHECK}
`define UINSTR_BRANCH           {`RT_12,                `SEQ_CONTROL_RESET}
`define UINSTR_JAL              {`RT_5 | `RT_12,        `SEQ_CONTROL_RESET}
`define UINSTR_JALR             {`RT_5 | `RT_13,        `SEQ_CONTROL_RESET}
`define UINSTR_IEX              {`RT_14,                `SEQ_CONTROL_DISPATCH_IEX}
`define UINSTR_NOP              {`RT_6,                 `SEQ_CONTROL_RESET}

// u-instructions uPC
`define UPC_SIZE                4
`define UINSTR_PC_IF            4'b0000
`define UINSTR_PC_ID            4'b0001   
`define UINSTR_PC_REX           4'b0010
`define UINSTR_PC_RWB           4'b0011
`define UINSTR_PC_MEM_ADDR      4'b0100
`define UINSTR_PC_LD_MEM_READ   4'b0101
`define UINSTR_PC_LD_WB         4'b0110
`define UINSTR_PC_SD_MEM_WRITE  4'b0111
`define UINSTR_PC_B_CHECK       4'b1000
`define UINSTR_PC_BRANCH        4'b1001
`define UINSTR_PC_JAL           4'b1010
`define UINSTR_PC_JALR          4'b1011
`define UINSTR_PC_IEX           4'b1100
`define UINSTR_PC_NOP           4'b1101

