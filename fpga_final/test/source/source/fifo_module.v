`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/16 17:36:52
// Design Name: 
// Module Name: fifo_module
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module fifo_module(
    input wr_clk ,//写 FIFO 时钟
    input rd_clk ,//读 FIFO 时钟
    input rst_n ,//复位
    input [7:0] wr_din ,//写入 FIFO 的数据
    input wr_en ,//写使能
    input rd_en ,//读使能
    output wr_full ,
    output reg [63:0] rd_dout,//从 FIFO 读出的数据
    output reg rd_out_vld,    //从 FIFO 读出的数据有效指示信号
    output  full_pos_edge,
    output  empty_pos_edge
 );
    //信号定义
    wire [7:0] wr_data ;
    wire [63:0] q ;
 
    wire wr_req ;
    wire rd_req ;
 
    wire rd_empty ;

 
    wire [12:0] wrusedw ;
    wire [9:0] rdusedw ;
    
    //FIFO 例化
     DRMFIFO  fifo_generator_0_u(
  .wr_clk(wr_clk),                // input
  .wr_rst(~rst_n),                // input
  .wr_en(wr_req),                  // input
  .wr_data(wr_data),              // input [8:0]
  .wr_full(wr_full),              // output
  .almost_full(wrusedw),      // output
  .rd_clk(rd_clk),                // input
  .rd_rst(~rst_n),                // input
  .rd_en(rd_req),                  // input
  .rd_data(q),              // output [63:0]
  .rd_empty(rd_empty),            // output
  .almost_empty(rdusedw)     // output

);

     reg full_first;
     reg wr_flag;
     assign wr_data = wr_din;//输入的数据
     assign wr_req = (wr_full  == 1'b0 && ~full_first && wr_flag)?wr_en:1'b0;//非满才写
     assign rd_req = (rd_empty == 1'b0)?rd_en:1'b0;//非空才读
     
     reg [1:0] D1,D2;
     
     always @(posedge wr_clk or negedge rst_n)begin
	    if(rst_n == 1'b0)begin
	        D1 <= 2'b11;
	        D2 <= 2'b11;
	    end
	    else begin
	        D1 <= {D1[0], wr_full};  	//D[1]表示前一状态，D[0]表示后一状态（新数据） 
	        D2 <= {D2[0], rd_empty};  	//D[1]表示前一状态，D[0]表示后一状态（新数据）
	    end
	end
	//组合逻辑决定是否写入
     always @(posedge full_pos_edge or posedge empty_pos_edge or negedge rst_n)begin
        if(rst_n == 1'b0)	 
            wr_flag<=1;
        else if(full_pos_edge)
            wr_flag<=0;
        else if(empty_pos_edge)
            wr_flag<=1;
	end

//组合逻辑进行边沿检测

	assign  full_pos_edge = ~D1[1] & D1[0];
	assign  empty_pos_edge = ~D2[1] & D2[0];

    always @(posedge wr_clk or negedge rst_n)begin
	    if(rst_n == 1'b0)begin
	       full_first <= 0;
	    end
	    else if(full_pos_edge)begin
	       full_first <= 1;
	    end
	    else if(full_first && rdusedw < 10'd64) begin
	       full_first <= 0;
	    end
	 end
     
     always @(posedge rd_clk or negedge rst_n)begin
        if(!rst_n)begin
         rd_dout <= 0;
        end
         else begin
         rd_dout <= q;
        end
     end
     always @(posedge rd_clk or negedge rst_n)begin
        if(!rst_n)begin
            rd_out_vld <= 1'b0;
        end
        else begin
            rd_out_vld <= rd_req;
        end
     end
 endmodule
