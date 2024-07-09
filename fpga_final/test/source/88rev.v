module rev88(
    input [63:0] in,
	output [63:0] out
   );

reg [63:0] out1;
    integer i;
    integer j;
    always @(in)
        begin
            for(i=0;i<=7;i=i+1)
               begin
                    for(j=0;j<=7;j=j+1)
                        begin
                            out1[63-i*8-j]<=in[7+8*i-j];
                        end
                end
        end
    assign out=out1;

endmodule