`timescale 1ns / 1ps 
odule lab3_2(
    			input [4:0] money,
			input CLK, 
			input [1:0] selectedArea, //00:exit, 01:enter, 1x: idle 
    			input plugAvailability,
			output reg [5:0] moneyLeft,
			output reg [5:0] seatLeft,
			output reg seatUnavailable,
    			output reg insufficientFund,
    			output reg notExactFund,
    			output reg invalidPlugSeat,
    			output reg plugSeatUnavailable,
    			output reg seatReady
	);
	reg [5:0] loudLeft = 15;
	reg [5:0] quietPluggedLeft = 10;
	reg [5:0] quietNonPluggedLeft = 15;
	reg [5:0] individualLeft = 20;
	reg [5:0] loudFee = 10, quietFee = 20, individualFee = 30;
	// IMPLEMENT BELOWS
	
	initial begin
			 moneyLeft <= 0;
			 seatUnavailable <= 0;
			 insufficientFund <= 0;
			 notExactFund <= 0;
			 invalidPlugSeat <= 0;
			 plugSeatUnavailable <= 0;
			 seatReady <= 0;
			 seatLeft <= 0;
	end
		
    always @(posedge CLK) begin
		 moneyLeft <= money;
		 seatUnavailable <= 0;
		 insufficientFund <= 0;
		 notExactFund <= 0;
		 invalidPlugSeat <= 0;
		 plugSeatUnavailable <= 0;
		 seatReady <= 0;
		if(selectedArea == 2'b00) begin
			if(loudLeft == 6'b000000)begin
				seatLeft <= loudLeft;
				seatUnavailable <= 1;
			end
			else begin
				if(plugAvailability == 1) begin
					invalidPlugSeat <= 1;
					seatLeft <= loudLeft;
				end
				else begin 
					if(money < loudFee)begin
						insufficientFund <= 1;
						seatLeft <= loudLeft;
					end
					else begin
						loudLeft = loudLeft - 6'b000001;
						seatLeft <= loudLeft;
						moneyLeft <= money - loudFee;
						seatReady <= 1;
					end
				end
			end
		end
		else if(selectedArea == 2'b01) begin
			if(plugAvailability == 0 && quietNonPluggedLeft == 6'b000000)begin
				seatUnavailable <= 1;
				seatLeft <= quietNonPluggedLeft;
			end
			else if(plugAvailability == 0) begin
				if(money < quietFee)begin
					insufficientFund <= 1;
					seatLeft <= quietNonPluggedLeft;
				end
				else begin
					quietNonPluggedLeft = quietNonPluggedLeft - 6'b000001;
					seatLeft <= quietNonPluggedLeft;
					moneyLeft <= money - quietFee;
					seatReady <= 1;
				end
			end 
			else if(plugAvailability == 1 && quietPluggedLeft == 6'b000000)begin
				plugSeatUnavailable <= 1;
				seatLeft <= quietPluggedLeft;
			end
			else begin
				if(money < quietFee)begin
					insufficientFund <= 1;
					seatLeft <= quietPluggedLeft;
				end
				else begin
					quietPluggedLeft = quietPluggedLeft - 6'b000001;
					seatLeft <= quietPluggedLeft;
					moneyLeft <= money - quietFee;
					seatReady <= 1;
				end
			end
		end
		else begin
			if(individualLeft == 6'b000000)begin
				seatUnavailable <= 1;
				seatLeft <= individualLeft;
			end
			else begin
				if(money == individualFee) begin
					individualLeft = individualLeft - 6'b000001;
					seatLeft <= individualLeft;
					moneyLeft <= 6'b000000;
					seatReady <= 1;
				end
				else 
					notExactFund <= 1;
					seatLeft <= individualLeft;
			end
		end
	end
endmodule
