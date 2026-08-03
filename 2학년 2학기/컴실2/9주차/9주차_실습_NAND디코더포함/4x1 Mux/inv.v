// 4x1 Mux

`timescale 1ns / 1ps

module inv (
    input a, b,
    input A, B, C, D,
    output OUT
    );
    
    
    assign OUT = (A & ~a & ~b) | (B & a & ~b) | (C & ~a & b) | (D & a & b);
    
    
endmodule