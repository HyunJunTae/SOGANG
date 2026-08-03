// D Flip-Flop

`timescale 1ns / 1ps

module inv (
    input  d, e,
    output reg q, nq
    );
    
    always @(posedge e) begin
        q <= d;
        nq <= ~d;
    end
    
    
endmodule