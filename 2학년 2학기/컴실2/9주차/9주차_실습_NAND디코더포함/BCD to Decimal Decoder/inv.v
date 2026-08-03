// BCD to Decimal Decoder

`timescale 1ns / 1ps

module inv (
    input  a, b, c, d, // 1 2 4 8 ¼ø
    output E0, E1, E2, E3, E4, E5, E6, E7, E8, E9
    );
    
    assign E0 = ~a & ~b & ~c & ~d;
    assign E1 = ~a & ~b & ~c & d;
    assign E2 = ~a & ~b & c & ~d;
    assign E3 = ~a & ~b & c & d;
    assign E4 = ~a & b & ~c & ~d;
    assign E5 = ~a & b & ~c & d;
    assign E6 = ~a & b & c & ~d;
    assign E7 = ~a & b & c & d;
    assign E8 = a & ~b & ~c & ~d;
    assign E9 = a & ~b & ~c & d;
    
    
endmodule