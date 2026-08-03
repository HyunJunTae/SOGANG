// FULL-ADDER

`timescale 1ns / 1ps

module inv (
    input a,
    input b,
    input c_in,
    output s,
    output c_out
    );

    assign s = a ^ b ^ c_in;
    assign c_out = c_in & (a^b) | (a&b);
    
endmodule