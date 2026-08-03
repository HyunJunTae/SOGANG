// BOOLEAN FUNCTION 2 - B

`timescale 1ns / 1ps

module inv_tb;

reg a,b,c;
wire d;
inv u_inv (
    .a(a ),
    .b(b ),
    .c(c ),
    .d(d )
);

integer i;

initial begin
    a = 1'b0;
    b = 1'b0;
    c = 1'b0;
    
    for (i = 0; i < 8; i = i + 1) begin
         {a, b, c} = i;
         #50;
    end
    
    #100;
        $finish;
end

endmodule
