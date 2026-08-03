// 2x4 Decoder AND

`timescale 1ns / 1ps

module inv_tb;

reg a,b;
wire D3, D2, D1, D0;

inv u_inv (
    .a(a),
    .b(b),
    .D3(D3),
    .D2(D2),
    .D1(D1),
    .D0(D0)
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
