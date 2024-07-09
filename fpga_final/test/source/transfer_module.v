`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/16 21:07:38
// Design Name: 
// Module Name: transfer_module
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


module transfer_module(
    input clk_1,
    input clk_2,
    input clk_3,
    input [63:0] data_fx,
    input clk,
    input rst,
    output adc_data
    );
    reg [2:0] flag;
    
    always @(posedge clk or negedge rst)
    begin
    if(!rst)
        flag <= 0;
    else begin
        if(data_fx>64'd99&&data_fx<64'd100_000)
             flag <= 0;
        else if(data_fx>64'd99_999)
             flag <= 1;
        else if(data_fx<64'd100)
             flag<=2;
    end
    end
    assign adc_data = (flag==0) ? clk_1:
            (flag==1) ? clk_2:
            (flag==2) ? clk_3:
            4'b0;

endmodule
