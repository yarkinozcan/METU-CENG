`timescale 1ns / 1ps

module ROM (
    input [2:0] addr,
    output reg [7:0] dataOut
);
	always @(*) begin
		if(addr == 0)
			dataOut <= 8'b00000000;
		else if(addr == 1)
			dataOut <= 8'b00000001;
		else if(addr == 2)
			dataOut <= 8'b00000010;
		else if(addr == 3)
			dataOut <= 8'b00100011;
		else if(addr == 4)
			dataOut <= 8'b11000100;
		else if(addr == 5)
			dataOut <= 8'b01100101;
		else if(addr == 6)
			dataOut <= 8'b00011110;
		else
			dataOut <= 8'b01000101;	
	end
endmodule



module Bitwise_Manipulation_RAM (
    input mode,                     // 0 for write, 1 for read
    input [2:0] addr,               // RAM address
	 input [1:0] operation,          // Operation code (AND, OR, XOR, NAND)
    input [7:0] dataIn,             // Input data
    input [7:0] romData,            // Data from ROM used in operations
    input CLK,                      // Clock signal
	 output reg [7:0] dataOut        // Output data
);
	reg [7:0] memory[0:7];
	reg [7:0] combined;
	integer i;
	initial begin
		combined = 0;
		for(i = 0 ; i < 8 ; i = i + 1)begin
			memory[i] = 0;
		end
	end
	always @(posedge CLK)begin
		if (mode == 0) begin
            case(operation)
                2'b00: combined = (dataIn & romData);
                2'b01: combined = (dataIn | romData);
                2'b10: combined = (dataIn ^ romData);
                2'b11: combined = ~(dataIn & romData);
            endcase
            memory[addr] <= combined;
            dataOut <= combined;
        end
	end
	always @(*) begin
		if(mode == 1)begin
			dataOut <= memory[addr];
		end
	end
endmodule



module Combined_Memory_System (
    input mode,
    input [2:0] systemAddr,
    input [7:0] dataIn,
    input [1:0] operation,
	 input CLK,
    output [7:0] systemOutput
);
	wire [7:0] romData; // Data read from ROM

	// instantiate ROM
	ROM rom(
	.addr(systemAddr),
	.dataOut(romData)
	);
	
	// instantiate RAM
	Bitwise_Manipulation_RAM ram(
	.mode(mode),
	.addr(systemAddr),
	.operation(operation),
	.dataIn(dataIn),
	.romData(romData),
	.CLK(CLK),
	.dataOut(systemOutput)
	);
endmodule

