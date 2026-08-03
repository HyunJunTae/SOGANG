// 4-bit Subtractor
`timescale 1ns / 1ps

module inv_tb;

    reg [3:0] a;
    reg [3:0] b;
    reg Bin;

    wire [3:0] Bout;
    wire [3:0] d;

    inv u_inv (
        .a(a),
        .b(b),
        .Bin(Bin),
        .Bout(Bout),
        .d(d)
    );

    integer i, j;

    initial begin
        a   = 4'b0000;
        b   = 4'b0000;
        Bin = 1'b0;

        for (i = 0; i < 16; i = i + 1) begin
            for (j = 0; j < 16; j = j + 1) begin
                a = i[3:0];
                b = j[3:0];
                Bin = 0;
                #25;
                Bin = 1;
                #25;
            end
        end

        #25;
        $finish;
    end

endmodule