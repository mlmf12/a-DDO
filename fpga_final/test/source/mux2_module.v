`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/16 19:58:34
// Design Name: 
// Module Name: mux2_module
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


module mux2_module(
	input  [63:0]in_f,
	input  [63:0]in_ad,//�����������
	input  rd,
	input  rst_n,
//	input  sel,//����ѡ���
    output reg flag_f,
	output reg [63:0]out//����źŶ�
);
reg [9:0] cnt;
reg sel;
	always @ (posedge rd or negedge rst_n)
	if(!rst_n)begin
        cnt <= 0;
        out <= 0;
        flag_f <= 1;
    end
//    else if(cnt == 0) begin
//        out <= in_f;
//        cnt <= cnt + 1;
//    end
	else if(cnt == 199) begin
		cnt <= 0;
		out <= in_f;
		flag_f <= 0;
	end
    else begin
	    out <= in_ad;
	    cnt <= cnt + 1;
	    flag_f <= 1;
	end
endmodule

