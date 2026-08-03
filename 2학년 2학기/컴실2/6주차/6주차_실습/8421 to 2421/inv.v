// 8421 to 2421

`timescale 1ns / 1ps

module inv (
    input a,
    input b,
    input c,
    input d,
    output e,
    output f,
    output g,
    output h
    );
    
    assign e = a | b&c | b&d;
    assign f = a | b&c | b&(~d);
    assign g = a | ~b&c | b&(~c)&d;
    assign h = d;
    
endmodule