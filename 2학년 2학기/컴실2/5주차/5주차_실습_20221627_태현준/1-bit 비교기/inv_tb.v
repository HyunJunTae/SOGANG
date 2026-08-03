// 1-bit Comparing

`timescale 1ns / 1ps

module inv_tb;

reg a,b;
wire c, d, e, f;
inv u_inv (
    .a(a ),
    .b(b ),
    .c(c ),
    .d(d ),
    .e(e ),
    .f(f )
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