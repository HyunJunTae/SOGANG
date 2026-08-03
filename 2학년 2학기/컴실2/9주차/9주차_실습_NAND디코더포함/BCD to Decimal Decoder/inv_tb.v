// BCD to Decimal Decoder

`timescale 1ns / 1ps

module inv_tb;

reg a, b, c, d;
wire E0, E1, E2, E3, E4, E5, E6, E7, E8, E9;

inv u_inv (
    .a(a),
    .b(b),
    .c(c),
    .d(d),
    .E0(E0),
    .E1(E1),
    .E2(E2),
    .E3(E3),
    .E4(E4),
    .E5(E5),
    .E6(E6),
    .E7(E7),
    .E8(E8),
    .E9(E9)
);

integer i;

initial begin
    a = 1'b0;
    b = 1'b0;
    c = 1'b0;
    d = 1'b0;
    
    for (i = 0; i < 16; i = i + 1) begin
         {a, b, c, d} = i;
         #50;
    end
    
    #100;
        $finish;
end

endmodule
