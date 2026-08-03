// RS-Latch NAND

`timescale 1ns / 1ps

module inv (
    input  s, e, r,
    output q, nq
    );
    
    assign q  = ~(~(e & s) & nq);
    assign nq = ~(~(e & r) & q );

endmodule