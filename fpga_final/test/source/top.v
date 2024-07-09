module top(
    input                       sys_clk,
	input                       rst,
	input[11:0]                 adc_data2,
	input[11:0]                 adc_data1,
	output reg [3:0]                    adc_data_reve,
    output reg     LEDTEST,
    output  adc_clk,
    output  adc_clk2,
    input clk_fx,
    output                          uart_tx,
    output test,

     input                 sclk_i       ,   //SCK
     input                 cs_n_i       ,   //CS_n
     input                 mosi_i       ,   //MOSI
     output                miso_o,          //MISO
     //output                motest
    input fifo_rst_n
   );
//assign test = adc_clk;
assign adc_clk2 = adc_clk;
//assign motest = 0;
wire miso_o;

//AD1
wire                            adc2_buf_wr;
wire[10:0]                      adc2_buf_addr;
wire[7:0]                       adc2_buf_data;


//����
localparam                       IDLE =  0;
localparam                       SEND =  1;   //send HELLO ALINX\r\n
localparam                       WAIT =  2;   //wait 1 second and send uart received data
reg[7:0]                         tx_data;
reg[7:0]                         tx_str;//synthesis syn_keep=1
reg                              tx_data_valid;
wire                             tx_data_ready;
reg[7:0]                         tx_cnt;
wire[7:0]                        rx_data;
wire                             rx_data_valid;
wire                             rx_data_ready;
reg[31:0]                        wait_cnt;
reg[3:0]                         state;


//100kHz����
divSquare divsquare
(
    .clk                    (clk_50m                  ),
    .rst_n                   (locked                ),
    .clk_out                   (test              )
);

//FIFO���
wire fifo_rd_rst;
wire rstFlag;
FIFORST fiforst(
    .rd_en(rd_en),
    .rst_n(rst),
    .clk(clk_200m),
    .rstFlag(rstFlag),
    .fifo_rd_rst(fifo_rd_rst)
   );

//���ȷ�ת��ʾ
wire [63:0] tempout;

rev88 rev88
(
    .in                    (tempout                  ),
    .out                   (rd_dout               )
);


   //AD����ģ��
wire [7:0] data_reg ;
wire fifo_rst;
ADC_get_module adget(
   .ad0_clk     (adc_clk)  ,
   .ad_data     (adc_data1[11:4])  ,
   .rst_n       (rst)  ,
   .fifo_rst    ()  ,
   .data_reg    (data_reg)  ,
   .wr_en       (wr_en));

//    FIFOģ��
wire full_pos_edge;
wire empty_pos_edge;
fifo_module fifo(
    .wr_clk (adc_clk) ,//д FIFO ʱ��
    .rd_clk (clk_200m),//�� FIFO ʱ��
    .rst_n  (rst),//��λ
    .wr_din (data_reg) ,//д�� FIFO ������
    .wr_en  (wr_en&&rstFlag),//дʹ��
    .rd_en  (rd_en && flag_f),//��ʹ��
    .rd_dout(tempout),//�� FIFO ����������
    .full_pos_edge(),
    .empty_pos_edge(empty_pos_edge),
    .wr_full(full_pos_edge)
 );


//Ƶ��ֵ��AD�����л�ģ��
wire flag_f;
wire [63:0] out;
wire[63:0] rd_dout;
//assign rd_dout = 64'b01111111_01111111_01111111_01111111_01111111_01111111_01111111_01111111;
mux2_module mux(
	.in_f      (data_fx)    ,
	.in_ad     (rd_dout)   ,//�����������
	.rd        (rd_en)    ,
	.rst_n     (locked)    ,
//	sel,//����ѡ���
    .flag_f    (flag_f)    ,
	.out       (out)//����źŶ�
);

wire ad_clk_1;
wire ad_clk_2;
wire ad_clk_3;
//�л�����Ƶ��

transfer_module trans(
    .data_fx(data_fx),
    .clk_1(ad_clk_1),
    .clk_2(ad_clk_2),
    .clk_3(ad_clk_3),
    .clk(sys_clk),
    .rst(rst),
    .adc_data(adc_clk)
    );

//assign adc_clk = ad_clk_3;
//��Ч����ʱ��ģ��
wire ad_clk_1;
ad_delay_module ad_delay(
    .clk_20b   (clk_200m),
    .rst_n      (rst),
    .data_fx    (data_fx),
//    .locked_ad    (locked_ad),
    .ad_clk     (ad_clk_2));

//ֱ�Ӳ���1kHz
div2 divadf2
(
    .clk                    (clk_50m                  ),
    .rst_n                   (locked                ),
    .clk_out                   (ad_clk_3              )
);
//ֱ�Ӳ���1MHz
div divadf
(
    .clk                    (clk_50m                  ),
    .rst_n                   (locked                ),
    .clk_out                   (ad_clk_1              )
);

divFIFO divfifo
(
    .clk                    (clk_50m                  ),
    .rst_n                   (locked                ),
    .clk_out                   (fifo_rst              )
);
//F_Measure
wire [63:0]   data_fx;//Frequency Data
reg [63:0]   data_fx_reg;
wire          flag;
parameter  CLK_FRE = 50;//Mhz
cymometer_direct f_measure
(
    .sys_clk                    (clk_50m                  ),
    .sys_rst_n                   (locked                ),
    .clk_fx                   (clk_fx              ),
    .fre                   (data_fx              )
);

an9238_sample ad9226_sample_m2(
	.adc_clk               (adc_clk                    ),
	.rst                   (~rst                     ),
	.adc_data              (adc_data            ),
	.adc_buf_wr            (adc1_buf_wr                ),
	.adc_buf_addr          (adc1_buf_addr              ),
	.adc_buf_data          (adc1_buf_data              )
);




//SPI �ӻ�ģ��

//assign out = 64'b11111111_11111111_11111111_11111111_11111111_11111111_11111111_11111111;
//assign out = 64'b00000000_00000000_00000000_00000000_11111111_00000000_11111111_00000000;
//assign out = 64'b00000000_00000000_00000000_00000000_00000000_00000000_00000000_11111111;
//assign out = 64'b01111111_01111111_01111111_01111111_01111111_01111111_01111111_01111111;
//assign out = 64'b00001111_00001111_00001111_00001111_00001111_00001111_00001111_00001111;
wire rd_en;
wire outdata;
SPI_slave_module spi_slave(
    .clk     (clk_200m),  //50MHzʱ��
    .rst_n   (locked),  //��λ
    .data_in (out),  //Ҫ���͵�����
    .data_out(data_rec),  //���յ�������
    .spi_sck (sclk_i),  //����ʱ��
    .spi_miso(miso_o),  //���մӷ����ӻ���
    .spi_mosi(mosi_i),  //�������գ��ӻ���
    .spi_cs  (cs_n_i),  //����Ƭѡ������Ч���ӻ���
    .tx_en   (rd_en),  //����ʹ��
    .tx_done (),  //������ɱ�־λ
    .rx_done ()   //������ɱ�־λ
);

    //���CS�����Բ�����ʹ���ź�ģ��
detect_module csget(
    .clk_200m(clk_200m),
    .rst_n   (locked),
    .cs_n_i  (cs_n_i),
    .rd_en   (rd_en));


    wire       clk_200m ;
    wire      clk_50m ;
    wire      clk_30m ;
    wire      clk_10m ;
    wire      clk_40m ;
    wire      locked;

clk_wiz_1 clk_wiz_0_inst
(
    .clkin1(sys_clk),            // IN 50Mhz

    .clkout0(clk_200m),                // OUT 200Mhz
    .clkout1(clk_50m),               // OUT 50Mhz
    .clkout2(clk_30m),              // OUT 30Mhz
    .clkout3(clk_10m ),    // OUT 10Mhz	 
    .clkout4(clk_40m ),    // OUT 40Mhz	
    .pll_rst(~rst),        // RESET IN
    .pll_lock(locked)	
); 

endmodule

