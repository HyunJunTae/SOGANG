// Decade Counter

`timescale 1ns / 1ps

module inv (clk, rst, out);

    output[3:0] out;
    reg[3:0] out;
    input clk, rst;
    
    
    always @(posedge clk)
        begin
            if(rst)
                out <= 0;
            else if (out == 4'b1001) // Make out 0 if out is already 9 and input is 1.
                out <= 0;
            else
                out <= out + 1;
        end
    
    
endmodule