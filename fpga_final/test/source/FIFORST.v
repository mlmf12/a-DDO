module FIFORST(
    input rd_en,
    input rst_n,
    input clk,
    output reg rstFlag,
    output reg fifo_rd_rst
   );

reg [31:0] num;
reg flag;
always @(posedge rd_en or negedge rst_n)begin
    if(~rst_n)
    begin
        num<=0;
    end
    else if(num>=500)
    begin
        num<=0;
    end
    else
    begin
        num<=num+1;
    end
    
end

always @(posedge clk or negedge rst_n)begin
    if(~rst_n)
    begin
        fifo_rd_rst<=1;
        rstFlag<=1;
    end
    else if(num>=500&&flag)
    begin
        fifo_rd_rst<=0;
        rstFlag<=0;
        flag<=0;
    end
    else
    begin
        fifo_rd_rst<=1;
        rstFlag<=1;
        flag<=1;
    end
    
end
endmodule