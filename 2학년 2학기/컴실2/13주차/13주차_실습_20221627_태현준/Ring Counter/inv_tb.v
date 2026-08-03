// Ring Counter
`timescale 1ns / 1ps

module inv_tb;

    reg clk, rst;
    wire [3:0] out;

    inv u_inv (
        .clk(clk),
        .rst(rst),
        .out(out)
    );

    always #10 clk = ~clk;
    
    initial begin
        
        clk = 0;
        rst = 0;
        
        #125;
        rst = 1;
        
        #40;
        rst = 0;
        
        #100;
        $finish;
    end

endmodule
