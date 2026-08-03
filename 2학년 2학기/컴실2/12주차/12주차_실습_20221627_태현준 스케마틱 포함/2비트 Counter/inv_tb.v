// 2-Bit Counter
`timescale 1ns / 1ps

module inv_tb;

    reg clk, rst;
    wire [1:0] out;

    inv u_inv (
        .clk(clk),
        .out(out),
        .rst(rst)
    );

    always #10 clk = ~clk;
    
    initial begin
        clk <= 1'b0;
        rst <= 1'b0;
        #20 rst <= 1'b1;
        #50 rst <= 1'b0;
        #80 rst <= 1'b1;
        #20 rst <= 1'b0;
        
        #100;
        $finish;
    end
    
    

endmodule