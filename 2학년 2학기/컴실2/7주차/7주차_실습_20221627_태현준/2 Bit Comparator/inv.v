// 2-bit comparator

`timescale 1ns / 1ps

module inv (
    input a,
    input b,
    input c,
    input d,
    output f1,
    output f2,
    output f3
    );
    
    assign f1 = (a&~c) | (b&~c&~d) | (a&b&~d);
    assign f2 = (~a & ~b & ~c & ~d) | (~a & b & ~c & d) | (a & ~b & c & ~d) | (a & b & c & d);
    assign f3 = (~a&c) | (~b&c&d) | (~a&~b&d);
    
endmodule