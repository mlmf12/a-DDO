`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/16 19:26:58
// Design Name: 
// Module Name: ADC_get_module
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


module ADC_get_module(
    input       ad0_clk         ,
    input [7:0] ad_data         ,
    input       rst_n           ,
    
    output     fifo_rst         ,
    output reg [7:0] data_reg   ,
    output reg  wr_en    
    
    );
    
//用来存储A/D转换后的数字量
    always@(posedge ad0_clk or negedge rst_n)
     begin
        if (!rst_n)
            data_reg <= 8'b0;
        else
            data_reg <= ad_data;
    end
    
    assign fifo_rst = (data_reg == 8'd209 && ad_data == 8'd209)? 0:1;
    
    reg [2:0] cnt;
    always@(posedge ad0_clk or negedge rst_n) begin
        if (!rst_n)
            cnt<=2'b0;
        else if (cnt==3'd5)
            cnt<=3'd5;
        else
            cnt<=cnt+1'b1;
    end
    always@(posedge ad0_clk or negedge rst_n) begin
//attention
        if (!rst_n) begin
            wr_en<=1'b0;
        end
        else if(cnt==3'd5) begin
            wr_en<=1'b1;
        end
        else
            wr_en<=1'b0;
    end

endmodule
