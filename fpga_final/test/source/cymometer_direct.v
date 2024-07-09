//ֱ�Ӳ�����(��Ƶ)
module	cymometer_direct(	
	input 				sys_clk		,		//��׼ʱ�ӣ����Ϊ50M(�ɸ���)
	input 				sys_rst_n	,		//��λ�źţ��͵�ƽ��Ч
	input 				clk_fx		,		//�����ź�
	output reg [31:0]	fre					//�������
);
 
parameter	TIME_SYS  = 20	;				//ϵͳʱ�����ڣ�20ns--Ƶ��=50MHz
parameter	TIME_GATE = 500_000_000	;		//500msբ�����õ�ʱ�䣬��λ��ns
localparam	N = TIME_GATE /	TIME_SYS;		//����բ����Ҫ�����ĸ���
 
reg 		gate		;					//բ��
reg [31:0] 	cnt_gate	;					//��������բ�ŵļ�����
reg [31:0] 	cnt_fx		;					//բ��ʱ���ڶԱ����źż���
 
wire		gate_n		;					//բ��ȡ���������ڷ�բ��ʱ�����õ�Ƶ��ֵ
 
assign	gate_n = ~gate	;					//բ��ȡ���������ڷ�բ��ʱ�����õ�Ƶ��ֵ
	
//��Ƶ��������բ��ʱ���趨Ϊ1ms����ÿ2ms����һ��	
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
 
//բ��ʱ���ڶԱ����źż���
always @(posedge clk_fx or negedge sys_rst_n)begin	
	if(!sys_rst_n)
		cnt_fx <= 0;
	else if(gate)
		cnt_fx <= cnt_fx + 1;
	else
		cnt_fx <= 0;
end
 
//�ڷ�բ��ʱ�����õ�Ƶ��ֵ
always @(posedge gate_n or negedge sys_rst_n)begin	
	if(!sys_rst_n)
		fre <= 0;
	else 
		//TIME_GATE/cnt_fx=�涨ʱ��/�����źŸ���=�����ź����ڣ�ȡ������ΪƵ��	
		fre <= 1000_000_000/TIME_GATE * cnt_fx;	
end
	
endmodule