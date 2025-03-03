module ALU #(parameter data_width = 16)
            (input [data_width-1:0] A,
             input [data_width-1:0] B,
             input [3:0] FuncCode,
             output reg [data_width-1:0] C,
             output reg OverflowFlag);
    
    `include "alu_func.v"
    
    initial begin
        C            = 0;
        OverflowFlag = 0;
    end
    
    always @(*) begin
        OverflowFlag = 0;
        
        case(FuncCode)
            `FUNC_ADD : begin
                C            = A+B;
                OverflowFlag = ~(A[data_width-1]^B[data_width-1])&(A[data_width-1]^C[data_width-1]);
            end
            `FUNC_SUB : begin
                C            = A-B;
                OverflowFlag = (A[data_width-1]^B[data_width-1])&(A[data_width-1]^C[data_width-1]);
            end
            `FUNC_ID  : C = A;
            `FUNC_NOT : C = ~A;
            `FUNC_AND : C = A&B;
            `FUNC_OR  : C = A|B;
            `FUNC_NAND: C = ~(A&B);
            `FUNC_NOR : C = ~(A|B);
            `FUNC_XOR : C = A^B;
            `FUNC_XNOR: C = ~(A^B);
            `FUNC_LLS : C = A<<1;
            `FUNC_LRS : C = A>>1;
            `FUNC_ALS : C = A<<<1;
            `FUNC_ARS : C = A>>>1;
            `FUNC_TCP : C = ~A+1;
            `FUNC_ZERO: C = 0;
        endcase
    end
    
endmodule
