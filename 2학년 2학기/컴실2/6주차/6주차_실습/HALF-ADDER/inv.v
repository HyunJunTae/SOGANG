// HALF-ADDER

`timescale 1ns / 1ps

module inv (
    input a,
    input b,
    output s,
    output c_out
    );

    assign s = a ^ b;
    assign c_out = a & b;
    
endmodule