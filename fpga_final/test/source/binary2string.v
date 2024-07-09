module binary_to_hex_string(
    input wire [31:0] binary_value,
    output reg [7:0] hex_string // 注意：这只是一个简化示例，并不能真正输出字符串
);

// Declare variables outside the always block
reg [3:0] nibbles[1:0]; // 存放两个半字节（nibbles）
reg [127:0] hex_chars; // 十六进制字符集
integer i;

initial begin
    hex_chars = 128'h30313233343536373839414243444546; // ASCII codes for "0123456789ABCDEF"
end

// 假设我们仅处理8位二进制数，并输出两位十六进制数
always @(*) begin
    // 将8位二进制数拆分为两个半字节
    nibbles[0] = binary_value[3:0]; // 低4位
    nibbles[1] = binary_value[7:4]; // 高4位
        
    // 将每个半字节转换为对应的十六进制字符
    for (i = 0; i < 2; i = i + 1) begin
        hex_string[i*4 +: 4] = hex_chars[nibbles[i]*8 +: 8]; // 将字符放入字符串的相应位置
    end
end

endmodule
