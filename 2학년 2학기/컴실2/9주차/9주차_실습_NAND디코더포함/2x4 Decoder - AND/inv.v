// 2x4 Decoder AND

`timescale 1ns / 1ps

module inv (
    input  a, b,
    output D3, D2, D1, D0
    );
    
    assign D3 = a & b;
    assign D2 = a & ~b;
    assign D1 = ~a & b;
    assign D0 = ~a & ~b;
    
endmodule