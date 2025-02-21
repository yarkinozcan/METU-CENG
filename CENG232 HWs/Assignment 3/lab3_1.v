`timescale 1ns / 1ps

module ac_flip_flop(
    input A,
    input C,
    input clk,
    output reg Q
);initial begin
	Q = 1;
	end
always @(posedge clk)
begin
	case ({A,C})
		2'b00: Q <= 1;
		2'b01: Q <= ~Q;
		2'b10: Q <= Q;
		2'b11: Q <= 1;
	endcase
end
endmodule

module ic1406(
    input A0,
    input A1,
    input A2,
    input clk,
    output Q0,
    output Q1,
    output Z
);
wire X, Y, L;
assign X = ~((A0 ^ A1) | ~A2);
assign Y = (A0 & A1);
assign L = (~(A0 & A1) & ~A2);
ac_flip_flop ff1(.A(X), .C(Y), .clk(clk), .Q(Q0));
ac_flip_flop ff2(.A(Y), .C(L), .clk(clk), .Q(Q1));
assign Z = (Q0 ^ Q1);

endmodule

