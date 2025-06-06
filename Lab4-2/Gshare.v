module Gshare(
    input clk,
    input reset,
    input is_stall,
    input [31:0] IF_pc,
    input ID_branch,
    input ID_bcond,
    input [31:0] IF_ID_pc,
    input [31:0] ID_next_pc,
    output reg [31:0] predicted_pc
);

    reg [24:0] TagTable[31:0];
    reg valid[31:0];
    reg [31:0] BTB[31:0];
    reg [1:0] PHT[31:0];
    reg [4:0] BHSR;

    integer i;

    wire [31:7] Tag = IF_pc[31:7];
    wire [31:7] update_Tag = IF_ID_pc[31:7];
    wire [4:0] BTB_idx = IF_pc[6:2];
    wire [4:0] update_BTB_idx = IF_ID_pc[6:2];
    wire [4:0] PHT_idx = BTB_idx ^ BHSR;
    wire [4:0] update_PHT_idx = update_BTB_idx ^ BHSR;

    always @(*) begin
        if ((TagTable[BTB_idx] == Tag) && valid[BTB_idx] && (PHT[PHT_idx] >= 2)) begin
            predicted_pc = BTB[BTB_idx];
        end
        else begin
            predicted_pc = IF_pc + 4;
        end
    end

    always @(posedge clk) begin
        if (reset) begin
            for (i=0; i<32; i=i+1) begin
                TagTable[i] <= 0;
                valid[i] <= 0;
                BTB[i] <= 0;
                PHT[i] <= 2;
            end
            BHSR <= 0;
        end
        else begin
            if (ID_branch && !is_stall) begin
                BHSR <= {ID_bcond, BHSR[4:1]};
                if (ID_bcond) begin
                    TagTable[update_BTB_idx] <= update_Tag;
                    valid[update_BTB_idx] <= 1;
                    BTB[update_BTB_idx] <= ID_next_pc;
                    case (PHT[update_PHT_idx])
                        0: PHT[update_PHT_idx] <= 1;
                        1: PHT[update_PHT_idx] <= 3;
                        2: PHT[update_PHT_idx] <= 3;
                        3: PHT[update_PHT_idx] <= 3;
                        default: PHT[update_PHT_idx] <= 2;
                    endcase
                end
                else begin
                    case (PHT[update_PHT_idx])
                        0: PHT[update_PHT_idx] <= 0;
                        1: PHT[update_PHT_idx] <= 0;
                        2: PHT[update_PHT_idx] <= 0;
                        3: PHT[update_PHT_idx] <= 2;
                        default: PHT[update_PHT_idx] <= 2;
                    endcase
                end
            end
        end
    end

endmodule
