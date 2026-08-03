// 4x1 Mux

`timescale 1ns / 1ps

module inv_tb;

reg a, b, A, B, C, D;
wire OUT;

inv u_inv (
    .a(a),
    .b(b),
    .A(A),
    .B(B),
    .C(C),
    .D(D),
    .OUT(OUT)
);

integer i;

initial begin
    a = 1'b0;
    b = 1'b0;
    A = 1'b0;
    B = 1'b0;
    C = 1'b0;
    D = 1'b0;
    
    
    for (i = 0; i < 64; i = i + 1) begin
         {a, b, A, B, C, D} = i;
         #50;
    end
    
    #5000000;
        $finish;
end

endmodule
