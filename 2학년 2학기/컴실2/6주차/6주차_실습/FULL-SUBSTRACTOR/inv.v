// FULL-SUBTRACTOR

`timescale 1ns / 1ps

module inv (
    input a,
    input b,
    input b_in,
    output d,
    output b_out
    );

    assign d = a ^ b ^ b_in;
    assign b_out = ~a&b_in | ~a&b | b&b_in;
    
endmodule