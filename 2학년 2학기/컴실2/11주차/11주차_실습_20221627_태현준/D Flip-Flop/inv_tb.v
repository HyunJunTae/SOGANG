// D Flip-Flop
`timescale 1ns / 1ps

module inv_tb;

    reg d, e;
    wire q, nq;

    inv u_inv (
        .d(d),
        .e(e),
        .q(q),
        .nq(nq)
    );
    
    
    initial begin
            e = 0;
            forever #20 e = ~e;  // 40ns period clock
        end
    
    integer i;
    
    initial begin
    
        for(i=0; i<2; i=i+1) begin
            {d} = i[0];
            @(posedge e);
            @(negedge e);
        end
        
        #25
        $finish;
    end

endmodule