module pll_1(
	input       sys_clk,
	input       rst_n,
	output      clk_200m ,
    output      clk_50m ,
    output      clk_30m ,
    output      clk_10m ,
	output locked          //pll clock output J8_Pin3
);


clk_wiz_1 clk_wiz_0_inst
(
    .clkin1(sys_clk),            // IN 50Mhz

    .clkout0(clk_200m),                // OUT 200Mhz
    .clkout1(clk_50m),               // OUT 100Mhz
    .clkout2(clk_30m),              // OUT 50Mhz
    .clkout3(clk_10m ),    // OUT 25Mhz	 

    .pll_rst(~rst_n),        // RESET IN
    .pll_lock(locked)	
); 
endmodule

