// 1x4 DeMux

`timescale 1ns / 1ps

module inv_tb;

reg a, b, F;
wire A, B, C, D;

inv u_inv (
    .a(a),
    .b(b),
    .F(F),
    .A(A),
    .B(B),
    .C(C),
    .D(D)
);

integer i;

initial begin
    a = 1'b0;
    b = 1'b0;
    F = 1'b0;
    
    
    for (i = 0; i < 8; i = i + 1) begin
         {a, b, F} = i;
         #50;
    end
    
    #1000000;
        $finish;
end

endmodule
