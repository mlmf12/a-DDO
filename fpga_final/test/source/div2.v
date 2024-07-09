module div2
#(
	parameter num = 32'd312
)
(
	input clk,
	input rst_n,
	output reg clk_out
);


reg [31:0]cnt;
always@(posedge clk or negedge rst_n)begin
    cnt <= cnt + 1'b1;
	if(!rst_n)begin
		cnt <= 0;
		clk_out <= 0;
	end
	else if(cnt >= num)begin
		cnt <= 0;
		clk_out <= 0; 
	end	
	else
		if(cnt >= num - 50)
            clk_out <= 1; 
end

endmodule
