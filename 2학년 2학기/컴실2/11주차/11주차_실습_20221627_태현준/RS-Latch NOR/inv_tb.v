// RS-Latch NOR
`timescale 1ns / 1ps

module inv_tb;

    reg s, r, e;
    wire q, nq;

    inv u_inv (
        .s(s),
        .e(e),
        .r(r),
        .q(q),
        .nq(nq)
    );
    
    
    initial begin
            e = 0;
            forever #25 e = ~e;  // 50ns period clock
        end
    
    integer i;
    
    initial begin
    
        for(i=0; i<4; i=i+1) begin
            {s, r} = i[1:0];
            @(posedge e);
            @(negedge e);
        end
        
        #25
        $finish;
    end

endmodule