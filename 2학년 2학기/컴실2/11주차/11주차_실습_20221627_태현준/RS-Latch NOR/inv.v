// RS-Latch NOR

`timescale 1ns / 1ps

module inv (
    input  s, e, r,
    output q, nq
    );
    
    assign q  = ~((e & r) | nq);
    assign nq = ~((e & s) | q);
    
endmodule