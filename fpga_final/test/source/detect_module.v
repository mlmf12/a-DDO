`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/16 19:52:50
// Design Name: 
// Module Name: detect_module
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


module detect_module(
    input       clk_200m    ,
    input       rst_n       ,
    input       cs_n_i      ,
    
    output      rd_en       
    
    );
    
    reg	spi_cs_r0,	spi_cs_r1;
  reg	spi_sck_r0,	spi_sck_r1;
    always @ ( posedge clk_200m or negedge rst_n )
    begin
	if ( !rst_n )
		begin
			spi_cs_r0	<= 1'b1;		
			spi_cs_r1	<= 1'b1;
		end
	else
		begin
			spi_cs_r0	<= cs_n_i;		
			spi_cs_r1	<= spi_cs_r0;
		end
    end

wire	mcu_cs	= spi_cs_r1;
wire	mcu_write_done = ( spi_cs_r0 & ~spi_cs_r1 ) ? 1'b1 : 1'b0;	//cs posedge capture
wire	mcu_write_start = ( ~spi_cs_r0 & spi_cs_r1 ) ? 1'b1 : 1'b0;	//cs negedge capture

assign rd_en = mcu_write_start;
    
endmodule
