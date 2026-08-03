// 1-bit Comparing

`timescale 1ns / 1ps

module inv (
    input a,
    input b,
    output c,
    output d,
    output e,
    output f
    );
    

    assign c = ~(a ^ b);    // A == B
    assign d = (a ^ b);     // A != B
    assign e = ~a & b;      // A < B
    assign f = a & ~b;      // A > B

    
endmodule
