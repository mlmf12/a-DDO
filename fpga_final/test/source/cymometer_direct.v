//直接测量法(高频)
module	cymometer_direct(	
	input 				sys_clk		,		//基准时钟，设计为50M(可更改)
	input 				sys_rst_n	,		//复位信号，低电平有效
	input 				clk_fx		,		//待测信号
	output reg [31:0]	fre					//测量结果
);
 
parameter	TIME_SYS  = 20	;				//系统时钟周期：20ns--频率=50MHz
parameter	TIME_GATE = 500_000_000	;		//500ms闸门设置的时间，单位：ns
localparam	N = TIME_GATE /	TIME_SYS;		//生成闸门需要计数的个数
 
reg 		gate		;					//闸门
reg [31:0] 	cnt_gate	;					//用于生成闸门的计数器
reg [31:0] 	cnt_fx		;					//闸门时间内对被测信号计数
 
wire		gate_n		;					//闸门取反，用于在非闸门时输出测得的频率值
 
assign	gate_n = ~gate	;					//闸门取反，用于在非闸门时输出测得的频率值
	
//分频计数器，闸门时间设定为1ms，则每2ms测量一次	
always @(posedge sys_clk or negedge sys_rst_n)begin	
	if(!sys_rst_n)begin
		cnt_gate <=0;
		gate <=0;
	end	
	else begin
		if(cnt_gate == N-1)begin
			cnt_gate <= 0;
			gate <= ~gate;
		end	
		else
		cnt_gate<=cnt_gate+1;
	end
end 
 
//闸门时间内对被测信号计数
always @(posedge clk_fx or negedge sys_rst_n)begin	
	if(!sys_rst_n)
		cnt_fx <= 0;
	else if(gate)
		cnt_fx <= cnt_fx + 1;
	else
		cnt_fx <= 0;
end
 
//在非闸门时输出测得的频率值
always @(posedge gate_n or negedge sys_rst_n)begin	
	if(!sys_rst_n)
		fre <= 0;
	else 
		//TIME_GATE/cnt_fx=规定时间/被测信号个数=被测信号周期，取倒数即为频率	
		fre <= 1000_000_000/TIME_GATE * cnt_fx;	
end
	
endmodule