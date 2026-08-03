// 4x2 Encoder

`timescale 1ns / 1ps

module inv (
    input  a, b, c, d,
    output E0, E1
    );
    
    assign E0 = (a & ~b & ~c & ~d) | (~a & b & ~c & ~d);
    assign E1 = (a & ~b & ~c & ~d) | (~a & ~b & c & ~d);
    
endmodule