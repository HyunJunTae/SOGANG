// 4-bit Subtractor
`timescale 1ns / 1ps

module inv (
    input  [3:0] a,     // 4-bit input A
    input  [3:0] b,     // 4-bit input B
    input        Bin,   // Borrow input
    output [3:0] Bout,  // Borrow Bit
    output [3:0] d      // 4-bit Difference output
    );

    assign d[0]  = a[0] ^ b[0] ^ Bin;
    assign Bout[0]  = (~a[0] & b[0]) | ((~(a[0] ^ b[0])) & Bin);

    assign d[1]  = a[1] ^ b[1] ^ Bout[0];
    assign Bout[1]  = (~a[1] & b[1]) | ((~(a[1] ^ b[1])) & Bout[0]);

    assign d[2]  = a[2] ^ b[2] ^ Bout[1];
    assign Bout[2]  = (~a[2] & b[2]) | ((~(a[2] ^ b[2])) & Bout[1]);

    assign d[3]  = a[3] ^ b[3] ^ Bout[2];
    assign Bout[3]  = (~a[3] & b[3]) | ((~(a[3] ^ b[3])) & Bout[2]);

endmodule