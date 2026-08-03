// 2-Bit Counter

`timescale 1ns / 1ps

module inv (clk, rst, out);

    output[1:0] out;
    reg[1:0] out;
    input clk, rst;
    
    
    always @(posedge clk)
        begin
            if(rst)
                out <= 0;
            else
                out <= out + 1;
        end
    
    
endmodule