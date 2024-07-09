module MyFIFO(
	input  rst_n,
	input  adc_clk,
    input[7:0]  adc_data2,
    //input[11:0]  adc_data2,
    output reg FIFO_Flag,
    output reg[63:0] rd_dout
   );


//合成幅度值
reg[4:0]                        num;
reg[63:0] rd_temp;
always@(posedge adc_clk or negedge rst_n)
begin
   
	if(~rst_n == 1'b1||num == 8/*4*/)
    begin
        num<=1'b0;
        FIFO_Flag<=0;
        rd_dout<=rd_temp;
        rd_temp<=64'b0;
    end
	else
    begin
/*
        if(num==0)
        begin
             num<=num+1;
             rd_temp[11:0]<=adc_data2;
        end
        else         if(num==1)
        begin
             num<=num+1;
             rd_temp[27:16]<=adc_data2;
        end
        else         if(num==2)
        begin
             num<=num+1;
             rd_temp[43:32]<=adc_data2;
        end
        else         if(num==3)
        begin
             num<=num+1;
             rd_temp[59:48]<=adc_data2;
        end

*/
        if(num==0)
        begin
             num<=num+1;
             rd_temp[7:0]<=adc_data2;
        end
        else         if(num==1)
        begin
             num<=num+1;
             rd_temp[15:8]<=adc_data2;
        end
        else         if(num==2)
        begin
             num<=num+1;
             rd_temp[23:16]<=adc_data2;
        end
        else         if(num==3)
        begin
             num<=num+1;
             rd_temp[31:24]<=adc_data2;
        end
        else         if(num==4)
        begin
             num<=num+1;
             rd_temp[39:32]<=adc_data2;
        end
        else         if(num==5)
        begin
             num<=num+1;
             rd_temp[47:40]<=adc_data2;
        end
        else         if(num==6)
        begin
             num<=num+1;
             rd_temp[55:48]<=adc_data2;
        end
        else         if(num==7)
        begin
             num<=num+1;
             rd_temp[63:56]<=adc_data2;
        end

    end
end
endmodule