// HALF-SUBTRACTOR

`timescale 1ns / 1ps

module inv (
    input a,
    input b,
    output d,
    output b_out
    );

    assign d = a ^ b;
    assign b_out = ~a & b;
    
endmodule