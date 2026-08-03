// UpDown Counter

`timescale 1ns / 1ps

module inv (clk, rst, updown, seg, digit, out);

    input clk, rst, updown;
    output reg [6:0] seg;
    output digit;
    output reg [3:0] out;
    
    assign digit = 1;
    
    initial out = 4'b0000;
    
    always @(posedge clk)
        begin
        
            if (updown == 0)
                seg = 7'b0111101;
            
            else
                seg = 7'b0111110;
                
                
                
            if(rst == 1'b1)
                out = 4'b0000;
            
            else if (updown == 1'b0) begin // down
                out <= out - 1;
            
            end
            
            else begin // up
                out <= out + 1;
                
            end
        end
    
    
endmodule