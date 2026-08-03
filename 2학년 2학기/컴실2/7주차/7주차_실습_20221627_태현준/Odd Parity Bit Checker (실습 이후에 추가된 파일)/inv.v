// parity bit checker

`timescale 1ns / 1ps

module inv (
    input a,
    input b,
    input c,
    input d,
    input p,
    output error
    );
    
    assign error = ~(a ^ b ^ c ^ d ^ p);
    
endmodule