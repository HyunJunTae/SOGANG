// Decade Counter
`timescale 1ns / 1ps

module inv_tb;

    reg clk, rst;
    wire [3:0] out;

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
        #80 rst <= 1'b0;
        #60 rst <= 1'b1;
        #40 rst <= 1'b0;
        
        #240;
        $finish;
    end
    
    

endmodule