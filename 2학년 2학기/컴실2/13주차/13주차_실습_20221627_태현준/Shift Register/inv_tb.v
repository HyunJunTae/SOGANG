// Shift Register
`timescale 1ns / 1ps

module inv_tb;

    reg clk, rst, in;
    wire [3:0] out;

    inv u_inv (
        .clk(clk),
        .rst(rst),
        .in(in),
        .out(out)
    );

    always #10 clk = ~clk;
    
    initial begin
        
        clk = 0;
        rst = 0;
        in  = 0;
        
        #45;
        in = 1;
        
        #40;
        in = 0;
        
        #60;
        in = 1;
        
        #100;
        rst = 1;
        
        #40;
        rst = 0;
        
        #60;
        in = 0;
        
        #100;
        $finish;
    end

endmodule
