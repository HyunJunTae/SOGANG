// 8421 to 2421

`timescale 1ns / 1ps

module inv_tb;

reg a, b, c, d;
wire e, f, g, h;
inv u_inv (
    .a(a ),
    .b(b ),
    .c(c ),
    .d(d ),
    .e(e ),
    .f(f ),
    .g(g ),
    .h(h )
);

integer i;

initial begin
    a = 1'b0;
    b = 1'b0;
    c = 1'b0;
    d = 1'b0;
    
    for (i = 0; i < 10; i = i + 1) begin
         {a, b, c, d} = i;
         #50;
    end
    
    #100;
        $finish;
end

endmodule
