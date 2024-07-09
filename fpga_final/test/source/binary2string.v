module binary_to_hex_string(
    input wire [31:0] binary_value,
    output reg [7:0] hex_string // ע�⣺��ֻ��һ����ʾ������������������ַ���
);

// Declare variables outside the always block
reg [3:0] nibbles[1:0]; // ����������ֽڣ�nibbles��
reg [127:0] hex_chars; // ʮ�������ַ���
integer i;

initial begin
    hex_chars = 128'h30313233343536373839414243444546; // ASCII codes for "0123456789ABCDEF"
end

// �������ǽ�����8λ�����������������λʮ��������
always @(*) begin
    // ��8λ�����������Ϊ�������ֽ�
    nibbles[0] = binary_value[3:0]; // ��4λ
    nibbles[1] = binary_value[7:4]; // ��4λ
        
    // ��ÿ�����ֽ�ת��Ϊ��Ӧ��ʮ�������ַ�
    for (i = 0; i < 2; i = i + 1) begin
        hex_string[i*4 +: 4] = hex_chars[nibbles[i]*8 +: 8]; // ���ַ������ַ�������Ӧλ��
    end
end

endmodule
