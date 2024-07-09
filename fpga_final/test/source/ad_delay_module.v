`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/16 20:26:53
// Design Name: 
// Module Name: ad_delay_module
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


module ad_delay_module(
    input           rst_n       ,
    input          clk_20b,
    input   [63:0]  data_fx     ,
    output          ad_clk    
    );
    wire clk_20b;
    wire locked_ad;
    assign locked_ad=rst_n;
    wire add_cnt0,add_cnt1,end_cnt0,end_cnt1;
    reg [63:0] cnt_0;
    
//计算延迟计数数量
    reg [63:0] N;
    always @(data_fx)
    begin
        N = (63'd200_000_000/data_fx);
        if((63'd2000_000_000/data_fx)%10'd10>5)
            N=N+1;
        //N<=200;
    end
    always @(posedge clk_20b or negedge locked_ad) begin
        if (!locked_ad) begin
            cnt_0 <= 0;
        end
        else if(add_cnt0) 
             begin
			     if(end_cnt0)
			     cnt_0 <= 0;
    		     else
			     cnt_0 <= cnt_0 + 1;
    		 end
        end
    assign add_cnt0 = 1 ;
	assign end_cnt0 = add_cnt0 && cnt_0 >= N;
    
    reg [24:0]cnt;	//定义计数器寄存器
	reg EN;

	always@(posedge clk_20b or negedge locked_ad) begin
	   if(!locked_ad)
		  EN  <= 1'b0;
	   else if(cnt_0>N/2)
		  EN  <= 1'b1;
	   else 
		  EN  <= 1'b0;
    end
assign ad_clk = EN;	

endmodule