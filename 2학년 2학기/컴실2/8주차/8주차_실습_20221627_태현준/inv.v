// 7-SEGMENT

`timescale 1ns / 1ps

module inv (
    input  a, b, c, d,  // 4-bit input
    output A, B, C, D, E, F, G, // 7-segment output
    output DP,           // DP output
    output digit
    );
    
    assign A = (~a & c) | (b & c) | (a & ~d) | (~a & b & d) | (a & ~b & ~c) | (~b & ~c & ~d);
    assign B = (~b & ~c) | (~a & ~c & ~d) | (a & ~c & d) | (~a & c & d) | (~b & c & ~d);
    assign C = (~a & b) | (a & ~b) | (~c & d) | (~a & ~c & ~d) | (~a & c & d);
    assign D = (a & ~c & ~d) | (~a & ~b & ~d) | (b & c & ~d) | (b & ~c & d) | (~b & c & d);
    assign E = (a & b) | (a & c) | (c & ~d) | (~b & ~c & ~d);
    assign F = (a & ~b) | (a & c) | (~c & ~d) | (~a & b & ~c) | (b & c & ~d);
    assign G = (a & ~b) | (a & d) | (c & ~d) | (~a & b & ~c) | (~a & ~b & c);
    
    assign DP = 1;
    assign digit = 1;
    
endmodule