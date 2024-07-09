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
    input wr_clk ,//д FIFO ʱ��
    input rd_clk ,//�� FIFO ʱ��
    input rst_n ,//��λ
    input [7:0] wr_din ,//д�� FIFO ������
    input wr_en ,//дʹ��
    input rd_en ,//��ʹ��
 
    output reg [63:0] rd_dout,//�� FIFO ����������
    output reg rd_out_vld,    //�� FIFO ������������Чָʾ�ź�
    output  full_pos_edge,
    output  empty_pos_edge
 );
    //�źŶ���
    wire [7:0] wr_data ;
    wire [63:0] q ;
 
    wire wr_req ;
    wire rd_req ;
 
    wire rd_empty ;
    wire wr_full ;
 
    wire [12:0] wrusedw ;
    wire [9:0] rdusedw ;
    
    //FIFO ����
FIFO fifo_generator_0 (
  .wr_data(wrusedw),              // input [15:0]
  .wr_en(wr_req),                  // input
  .wr_clk(wr_clk),                // input
  .full(wr_full),                    // output
  .wr_rst(~rst_n),                // input
  .almost_full(wr_full),      // output
  .rd_data(rdusedw),              // output [15:0]
  .rd_en(rd_req),                  // input
  .rd_clk(rd_clk),                // input
  .empty(rd_empty),                  // output
  .rd_rst(~rst_n),                // input
  .almost_empty(rd_empty)     // output
);
     reg full_first;

     assign wr_data = wr_din;//���������
     assign wr_req = (wr_full  == 1'b0 && ~full_first)?wr_en:1'b0;//������д
     assign rd_req = (rd_empty == 1'b0)?rd_en:1'b0;//�ǿղŶ�
     
     reg [1:0] D1,D2;
     
     always @(posedge wr_clk or negedge rst_n)begin
	    if(rst_n == 1'b0)begin
	        D1 <= 2'b11;
	        D2 <= 2'b11;
	    end
	    else begin
	        D1 <= {D1[0], wr_full};  	//D[1]��ʾǰһ״̬��D[0]��ʾ��һ״̬�������ݣ� 
	        D2 <= {D2[0], rd_empty};  	//D[1]��ʾǰһ״̬��D[0]��ʾ��һ״̬�������ݣ�
	    end
	end
	
//����߼����б��ؼ��

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
