// HALF-SUBTRACTOR

`timescale 1ns / 1ps

module inv_tb;

reg a,b;
wire d, b_out;
inv u_inv (
    .a(a ),
    .b(b ),
    .b_out(b_out ),
    .d(d )
);

integer i;

initial begin
    a = 1'b0;
    b = 1'b0;
    
    for (i = 0; i < 4; i = i + 1) begin
         {a, b} = i;
         #50;
    end
    
    #100;
        $finish;
end

endmodule
