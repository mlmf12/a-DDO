module divSquare
#(
	parameter num = 32'd500
)
(
	input clk,
	input rst_n,
	output reg clk_out
);


reg [31:0]cnt;
always@(posedge clk or negedge rst_n)begin
	if(!rst_n)begin
		cnt <= 0;
		clk_out <= 0;
	end
	else if(cnt == num/2)begin
		cnt <= 0;
		clk_out <= ~clk_out; 
	end	
	else
		cnt <= cnt + 1'b1;
end

endmodule
