`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Module Name:    volt_cal 
//////////////////////////////////////////////////////////////////////////////////
module volt_cal(
   input        ad_clk,                  //
	
	input [11:0] ad_ch1,              
	input [11:0] ad_ch2,              


	output [19:0] ch1_dec,              
	output [19:0] ch2_dec,              
   
	output reg [7:0] ch1_sig,               
	output reg [7:0] ch2_sig                

    );

reg [31:0] ch1_data_reg;
reg [31:0] ch2_data_reg;


reg [12:0] ch1_reg;
reg [12:0] ch2_reg;

reg [31:0] ch1_vol;              
reg [31:0] ch2_vol;              

//AD voltage negitive or positive check
always @(posedge ad_clk)
begin
    if(ad_ch1[11]==1'b1) begin                     //check the MSB bit, if MSB=1, the voltage is positive
	    ch1_reg<=ad_ch1 - 12'h800;
		 ch1_sig <= 43;                              //asic of '+'
	 end
	 else begin
       ch1_reg<=12'h800 - ad_ch1;
		 ch1_sig<=45;                                //asic of '-'
	 end	 
		 
    if(ad_ch2[11]==1'b1) begin                     //check the MSB bit, if MSB=1, the voltage is positive
	    ch2_reg<=ad_ch2 - 12'h800;
		 ch2_sig <= 43;                              //asic of '+'
	 end
	 else begin
       ch2_reg<=12'h800 - ad_ch2;
		 ch2_sig<=45;                                //asic of '-'
	 end	 
	 
 
end 		 

 
//AD Voltage converter (1 LSB = 5V / 2048 = 2.44mV
always @(posedge ad_clk)
begin
	ch1_data_reg<=ch1_reg * 5000;			
	ch2_data_reg<=ch2_reg * 5000;
			
   ch1_vol<=ch1_data_reg >>11;
   ch2_vol<=ch2_data_reg >>11;

end	

//16 hex converter to BCD code
bcd bcd1_ist(         
               .hex           (ch1_vol[15:0]),
					.dec           (ch1_dec),
					.clk           (ad_clk)
					);

//16 hex converter to BCD code
bcd bcd2_ist(         
               .hex           (ch2_vol[15:0]),
					.dec           (ch2_dec),
					.clk           (ad_clk)
					);
  
  
endmodule


  