// 2421 Counter

`timescale 1ns / 1ps

module inv (clk, rst, out);

    output[3:0] out;
    reg[3:0] out;
    input clk, rst;
    
    
    always @(posedge clk) begin
        if (rst)
            out <= 4'b0000;
        else begin
            case (out)
                4'b0000, 4'b0001, 4'b0010, 4'b0011: out <= out + 1;
                4'b0100: out <= 4'b1011;
                4'b1011: out <= 4'b1100;
                4'b1100: out <= 4'b1101;
                4'b1101: out <= 4'b1110;
                4'b1110: out <= 4'b1111;
                4'b1111: out <= 4'b0000;
            endcase
        end
    end
    
endmodule