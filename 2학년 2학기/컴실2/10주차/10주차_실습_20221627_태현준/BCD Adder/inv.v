// BCD Adder

`timescale 1ns / 1ps

module inv (
    input  [3:0] a,   // BCD input A
    input  [3:0] b,   // BCD input B
    input        Cin, // Carry in
    output [3:0] s,   // BCD sum output
    output       Cout // Carry out (to next digit)
    );

        wire [3:0] s1;
        wire [3:0] c1;
        
        wire correction_needed;
        wire [3:0] correction; // 0110
        
        wire [3:0] s2; 
        wire [3:0] c2;
        
    
        // 1st 4-bit Adder (Binary addition)
        assign s1[0] = a[0] ^ b[0] ^ Cin;
        assign c1[0]  = (a[0] & b[0]) | (a[0] & Cin) | (b[0] & Cin);
    
        assign s1[1] = a[1] ^ b[1] ^ c1[0];
        assign c1[1]  = (a[1] & b[1]) | (a[1] & c1[0]) | (b[1] & c1[0]);
    
        assign s1[2] = a[2] ^ b[2] ^ c1[1];
        assign c1[2]  = (a[2] & b[2]) | (a[2] & c1[1]) | (b[2] & c1[1]);
    
        assign s1[3] = a[3] ^ b[3] ^ c1[2];
        assign c1[3]  = (a[3] & b[3]) | (a[3] & c1[2]) | (b[3] & c1[2]);
        

        // Correction logic
        assign correction_needed = c1[3] | (s1[3] & s1[2]) | (s1[3] & s1[1]);
        assign correction = correction_needed ? 4'b0110 : 4'b0000;
    

        // 2nd 4-bit Adder (Add correction = +6 if needed)
        assign s2[0] = s1[0] ^ correction[0] ^ 1'b0;
        assign c2[0] = (s1[0] & correction[0]) | (s1[0] & 1'b0) | (correction[0] & 1'b0);
    
        assign s2[1] = s1[1] ^ correction[1] ^ c2[0];
        assign c2[1] = (s1[1] & correction[1]) | (s1[1] & c2[0]) | (correction[1] & c2[0]);
    
        assign s2[2] = s1[2] ^ correction[2] ^ c2[1];
        assign c2[2] = (s1[2] & correction[2]) | (s1[2] & c2[1]) | (correction[2] & c2[1]);
    
        assign s2[3] = s1[3] ^ correction[3] ^ c2[2];
        assign c2[3] = (s1[3] & correction[3]) | (s1[3] & c2[2]) | (correction[3] & c2[2]);
    
    
        // Final outputs
        assign s    = s2;
        assign Cout = correction_needed;
    
    
endmodule