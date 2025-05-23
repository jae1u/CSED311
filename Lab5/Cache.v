`include "CLOG2.v"
// This cache doesn't support full associativity. (NUM_SETS > 1)
module Cache #(parameter LINE_SIZE = 16,
               parameter NUM_SETS = 4,
               parameter NUM_WAYS = 4) (
    input reset,
    input clk,

    input is_input_valid,
    input [31:0] addr,
    input mem_read,
    input mem_write,
    input [31:0] din,

    output is_ready,
    output is_output_valid,
    output [31:0] dout,
    output is_hit);

  localparam LINE_BITS = `CLOG2(LINE_SIZE);
  localparam SET_BITS = `CLOG2(NUM_SETS);
  localparam WAY_BITS = `CLOG2(NUM_WAYS);
  localparam BLOCK_OFFSET_BITS = LINE_BITS - 2;
  localparam TAG_BITS = 32 - SET_BITS - LINE_BITS;
  localparam NUM_WORDS_PER_LINE = LINE_SIZE / 4;

  wire [TAG_BITS-1:0] tag = addr[31:32-TAG_BITS];
  wire [SET_BITS-1:0] set_index = (SET_BITS == 0) ? 0 : addr[32-TAG_BITS-1:LINE_BITS];
  wire [BLOCK_OFFSET_BITS-1:0] block_offset = addr[LINE_BITS-1:2];

  wire data_mem_is_output_valid;
  wire [LINE_SIZE*8-1:0] data_mem_dout;
  wire is_data_mem_ready;

  reg [LINE_SIZE*8-1:0] cache_data  [NUM_SETS-1:0][NUM_WAYS-1:0];
  reg [TAG_BITS-1:0]    cache_tag   [NUM_SETS-1:0][NUM_WAYS-1:0];
  reg                   cache_valid [NUM_SETS-1:0][NUM_WAYS-1:0];
  reg                   cache_dirty [NUM_SETS-1:0][NUM_WAYS-1:0];
  reg [WAY_BITS-1:0]    cache_lru   [NUM_SETS-1:0][NUM_WAYS-1:0];

  reg [31:0] _dout;
  reg        _is_hit;
  reg        _is_output_valid;
  reg [1:0]  state;
  reg [31:0] miss_addr;
  reg        miss_mem_write;
  reg        miss_mem_read;
  reg [31:0] miss_din;
  reg        data_mem_is_input_valid;
  reg [31:0] data_mem_addr;
  reg        data_mem_read;
  reg        data_mem_write;
  reg [LINE_SIZE*8-1:0] data_mem_din;
  reg [WAY_BITS-1:0] victim_way;

  assign is_ready = state == 0;
  assign is_output_valid = _is_output_valid;
  assign dout = _dout;
  assign is_hit = _is_hit;

  integer i, j, k, w;

  reg hit;
  reg [WAY_BITS-1:0] hit_way;
  always @(*) begin
    hit = 0;
    hit_way = 0;
    for (k = 0; k < NUM_WAYS; k = k + 1) begin
      if (cache_valid[set_index][k] && (cache_tag[set_index][k] == tag)) begin
        hit = 1;
        hit_way = k[WAY_BITS-1:0];
      end
    end
  end

  function automatic [WAY_BITS-1:0] get_victim_way;
    input [SET_BITS-1:0] set_idx;
    integer way;
    reg [WAY_BITS-1:0] lru_way;
    begin
      lru_way = 0;
      for (way = 1; way < NUM_WAYS; way = way + 1) begin
        if (cache_lru[set_idx][way] > cache_lru[set_idx][lru_way])
          lru_way = way[WAY_BITS-1:0];
      end
      for (way = NUM_WAYS - 1; way >= 0; way = way - 1) begin
        if (!cache_valid[set_idx][way])
          lru_way = way[WAY_BITS-1:0];
      end
      get_victim_way = lru_way;
    end
  endfunction

  task update_lru;
    input [SET_BITS-1:0] set_idx;
    input [WAY_BITS-1:0] way_idx;
    integer way;
    begin
      for (way = 0; way < NUM_WAYS; way = way + 1)
        if (way[WAY_BITS-1:0] == way_idx) begin
          cache_lru[set_idx][way] <= 0;
        end
        else if (cache_lru[set_idx][way] < cache_lru[set_idx][way_idx]) begin
          cache_lru[set_idx][way] <= cache_lru[set_idx][way] + 1;
        end
    end
  endtask

  always @(posedge clk) begin
    if (reset) begin
      for (i = 0; i < NUM_SETS; i = i + 1) begin
        for (j = 0; j < NUM_WAYS; j = j + 1) begin
          cache_valid[i][j] <= 0;
          cache_lru[i][j] <= j[WAY_BITS-1:0];
        end
      end
      state <= 0;
      data_mem_is_input_valid <= 0;
      _is_output_valid <= 0;
    end
    else begin
      case (state)
        0: begin
          if (is_input_valid && !_is_output_valid) begin
            if (mem_read && hit) begin
              for (w = 0; w < NUM_WORDS_PER_LINE; w = w + 1)
                if (block_offset == w[BLOCK_OFFSET_BITS-1:0]) begin
                  _dout <= cache_data[set_index][hit_way][w*32 +: 32];
                end
              update_lru(set_index, hit_way);
              _is_hit <= 1;
              _is_output_valid <= 1;
            end
            else if (mem_read && !hit) begin
              state <= 1;
              miss_addr <= addr;
              miss_mem_read <= 1;
              miss_mem_write <= 0;
              victim_way <= get_victim_way(set_index);
              _is_output_valid <= 0;
            end
            else if (mem_write && hit) begin
              for (w = 0; w < NUM_WORDS_PER_LINE; w = w + 1)
                if (block_offset == w[BLOCK_OFFSET_BITS-1:0]) begin
                  cache_data[set_index][hit_way][w*32 +: 32] <= din;
                end
              cache_dirty[set_index][hit_way] <= 1;
              update_lru(set_index, hit_way);
              _is_hit <= 1;
              _is_output_valid <= 1;
            end
            else if (mem_write && !hit) begin
              state <= 1;
              miss_addr <= addr;
              miss_mem_read <= 0;
              miss_mem_write <= 1;
              miss_din <= din;
              victim_way <= get_victim_way(set_index);
              _is_output_valid <= 0;
            end
          end
          else begin
            _is_output_valid <= 0;
          end
        end
        1: begin
          if (cache_valid[set_index][victim_way] && cache_dirty[set_index][victim_way]) begin
            data_mem_is_input_valid <= 1;
            data_mem_addr <= {cache_tag[set_index][victim_way], set_index, {LINE_BITS{1'b0}}} >> LINE_BITS;
            data_mem_read <= 0;
            data_mem_write <= 1;
            data_mem_din <= cache_data[set_index][victim_way];
            state <= 2;
          end
          else begin
            data_mem_is_input_valid <= 1;
            data_mem_addr <= miss_addr >> LINE_BITS;
            data_mem_read <= 1;
            data_mem_write <= 0;
            state <= 3;
          end
        end
        2: begin
          if (is_data_mem_ready && !data_mem_is_input_valid) begin
            data_mem_is_input_valid <= 1;
            data_mem_addr <= miss_addr >> LINE_BITS;
            data_mem_read <= 1;
            data_mem_write <= 0;
            state <= 3;
          end
          else begin
            data_mem_is_input_valid <= 0;
          end
        end
        3: begin
          if (data_mem_is_output_valid) begin
            cache_data[set_index][victim_way] <= data_mem_dout;
            cache_tag[set_index][victim_way] <= tag;
            cache_valid[set_index][victim_way] <= 1;
            update_lru(set_index, victim_way);
            if (miss_mem_read) begin
              for (w = 0; w < NUM_WORDS_PER_LINE; w = w + 1)
                if (block_offset == w[BLOCK_OFFSET_BITS-1:0]) begin
                  _dout <= data_mem_dout[w*32 +: 32];
                end
              cache_dirty[set_index][victim_way] <= 0;
              _is_output_valid <= 1;
              _is_hit <= 0;
            end
            else if (miss_mem_write) begin
              for (w = 0; w < NUM_WORDS_PER_LINE; w = w + 1)
                if (block_offset == w[BLOCK_OFFSET_BITS-1:0]) begin
                  cache_data[set_index][victim_way][w*32 +: 32] <= miss_din;
                end
              cache_dirty[set_index][victim_way] <= 1;
              _is_output_valid <= 1;
              _is_hit <= 0;
            end
            state <= 0;
          end
          else begin
            data_mem_is_input_valid <= 0;
          end
        end
        default: state <= 0;
      endcase
    end
  end

  // Instantiate data memory
  DataMemory #(.BLOCK_SIZE(LINE_SIZE)) data_mem(
    .reset(reset),
    .clk(clk),

    .is_input_valid(data_mem_is_input_valid),
    .addr(data_mem_addr),
    .mem_read(data_mem_read),
    .mem_write(data_mem_write),
    .din(data_mem_din),

    // is output from the data memory valid?
    .is_output_valid(data_mem_is_output_valid),
    .dout(data_mem_dout),
    // is data memory ready to accept request?
    .mem_ready(is_data_mem_ready)
  );
endmodule
