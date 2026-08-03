// UpDown Counter
`timescale 1ns / 1ps

module inv_tb;

    reg clk, rst, updown;
    wire [3:0] out;

    inv u_inv (
        .clk(clk),
        .rst(rst),
        .updown(updown),
        .out(out)
    );

    always #10 clk = ~clk;
    
    initial begin
        
        clk = 0;
        rst = 0;
        updown = 0;
        
        #65;
        updown = 1;
        
        #400;
        updown = 0;
        
        #60
        rst = 1;
        
        #40
        rst = 0;
        updown = 1;
        
        #60;
        $finish;
    end

endmodule
