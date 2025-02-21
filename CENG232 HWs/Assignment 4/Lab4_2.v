`timescale 1ns / 1ps

module lab4_2(

							input [1:0] mode,//Student Entrance Mode 01, Student Exit Mode 00,Search Mode 10,List Mode 11
							input [5:0] userID,//USERID
							input CLK,
							output reg [1:0] selectedAreaId,// show user selected area 
							output reg [5:0] numberOfInsideUser, //seatLeft
							output reg [5:0] listOutput,
							output reg AlreadyInside, //update 	
							output reg NotInside  //update 
    );

//locals
parameter loud = 2'b00, quiet = 2'b01, individual = 2'b10, zoom = 2'b11;

reg [15:0] seats[0:3];

reg [5:0] listCount[0:3];

reg [4:0] loudLeft = 0;
reg [4:0] quietLeft = 0;
reg [4:0] individualLeft = 0;
reg [4:0] zoomLeft = 0;

reg [4:0] loudList = 0;
reg [4:0] quietList = 0;
reg [4:0] individualList = 0;
reg [4:0] zoomList = 0;

integer i;
						

							initial begin
								AlreadyInside <= 0;
								NotInside <= 0;
								selectedAreaId <= 0;
								listOutput <= 0;
								numberOfInsideUser <= 0;
								for(i = 0 ; i < 4 ; i = i + 1)begin
									listCount[i] <= 0;
								end
								for(i = 0 ; i < 4 ; i = i + 1)begin
									seats[i] <= 0;
								end
							end
							always @(posedge CLK)
							begin
								AlreadyInside <= 0;
								NotInside <= 0;listOutput <= 0;
								numberOfInsideUser <= 0;
								if(mode == 0)begin
									quietList = 0;
									individualList = 0;
									loudList = 0;
									zoomList = 0;
									
									for(i = 0 ; i < 4 ; i = i + 1)begin
										listCount[i] <= 0;
									end
									
									selectedAreaId <= userID[5:4];
									
									if(userID[5:4] == loud)begin
										if(seats[loud][userID[3:0]] == 0)begin
											NotInside <= 1;
											numberOfInsideUser <= loudLeft;
										end
										else begin
											seats[loud][userID[3:0]] <= 0;
											loudLeft = loudLeft - 1;
											numberOfInsideUser <= loudLeft;
										end
									end
									
									if(userID[5:4] == quiet)begin
										if(seats[quiet][userID[3:0]] == 0)begin
											NotInside <= 1;
											numberOfInsideUser <= quietLeft;
										end
										else begin
											seats[quiet][userID[3:0]] <= 0;
											quietLeft = quietLeft - 1;
											numberOfInsideUser <= quietLeft;
										end
									end
									
									if(userID[5:4] == individual)begin
										if(seats[individual][userID[3:0]] == 0)begin
											NotInside <= 1;
											numberOfInsideUser <= individualLeft;
										end
										else begin
											seats[individual][userID[3:0]] <= 0;
											individualLeft = individualLeft - 1;
											numberOfInsideUser <= individualLeft;
										end
									end
									
									if(userID[5:4] == zoom)begin
										if(seats[zoom][userID[3:0]] == 0)begin
											NotInside <= 1;
											numberOfInsideUser <= zoomLeft;
										end
										else begin
											seats[zoom][userID[3:0]] <= 0;
											zoomLeft = zoomLeft - 1;
											numberOfInsideUser <= zoomLeft;
										end
									end
								end
								
								if(mode == 1)begin
									quietList = 0;
									individualList = 0;
									loudList = 0;
									zoomList = 0;
									
									for(i = 0 ; i < 4 ; i = i + 1)begin
										listCount[i] <= 0;
									end
									
									selectedAreaId <= userID[5:4];
									
									if(userID[5:4] == loud)begin
										if(seats[loud][userID[3:0]] == 0)begin
											loudLeft = loudLeft + 1;
											numberOfInsideUser <= loudLeft;
											seats[loud][userID[3:0]] <= 1;
										end
										else begin
											AlreadyInside <= 1;
											numberOfInsideUser <= loudLeft;
										end
									end
									
									if(userID[5:4] == quiet)begin
										if(seats[quiet][userID[3:0]] == 0)begin
											quietLeft = quietLeft + 1;
											numberOfInsideUser <= quietLeft;
											seats[quiet][userID[3:0]] <= 1;
										end
										else begin
											AlreadyInside <= 1;
											numberOfInsideUser <= quietLeft;
										end
									end
									
									if(userID[5:4] == individual)begin
										if(seats[individual][userID[3:0]] == 0)begin
											individualLeft = individualLeft + 1;
											numberOfInsideUser <= individualLeft;
											seats[individual][userID[3:0]] <= 1;
										end
										else begin
											AlreadyInside <= 1;
											numberOfInsideUser <= individualLeft;
										end
									end
									
									if(userID[5:4] == zoom)begin
										if(seats[zoom][userID[3:0]] == 0)begin
											zoomLeft = zoomLeft + 1;
											numberOfInsideUser <= zoomLeft;
											seats[zoom][userID[3:0]] <= 1;
										end
										else begin
											AlreadyInside <= 1;
											numberOfInsideUser <= zoomLeft;
										end
									end
								end
								
								if(mode == 2)begin
									quietList = 0;
									individualList = 0;
									loudList = 0;
									zoomList = 0;
									
									for(i = 0 ; i < 4 ; i = i + 1)begin
										listCount[i] <= 0;
									end
									
									selectedAreaId <= userID[5:4];
									
									if(userID[5:4] == loud)begin
										numberOfInsideUser <= loudLeft;
										if(seats[loud][userID[3:0]] == 0)begin
											NotInside <= 1;
										end
										else begin
											AlreadyInside <= 1;
										end
									end
									
									if(userID[5:4] == quiet)begin
										numberOfInsideUser <= quietLeft;
										if(seats[quiet][userID[3:0]] == 0)begin
											NotInside <= 1;
										end
										else begin
											AlreadyInside <= 1;
										end
									end
									
									if(userID[5:4] == individual)begin
										numberOfInsideUser <= individualLeft;
										if(seats[individual][userID[3:0]] == 0)begin
											NotInside <= 1;
										end
										else begin
											AlreadyInside <= 1;
										end
									end
									
									if(userID[5:4] == zoom)begin
										numberOfInsideUser <= zoomLeft;
										if(seats[zoom][userID[3:0]] == 0)begin
											NotInside <= 1;
										end
										else begin
											AlreadyInside <= 1;
										end
									end
								end
								
								if(mode == 3)begin
									selectedAreaId <= userID[5:4];
									if(userID[5:4] == loud)begin
										numberOfInsideUser <= loudLeft;
										
										quietList = 0;
										individualList = 0;
										zoomList = 0;
										listCount[quiet] = 0;
										listCount[individual] = 0;
										listCount[zoom] = 0;
										
										if(loudList >= loudLeft)begin
											listOutput <= 4'bxxxx;
										end
										else begin
											while(seats[loud][listCount[loud]] == 0)begin
												listCount[loud] = listCount[loud] + 1;
											end
											listOutput <= listCount[loud];
											listCount[loud] = listCount[loud] + 1;
											loudList = loudList + 1;
										end
									end
									
									if(userID[5:4] == quiet)begin
										numberOfInsideUser <= quietLeft;
										loudList = 0;
										individualList = 0;
										zoomList = 0;
										listCount[loud] = 0;
										listCount[individual] = 0;
										listCount[zoom] = 0;
										
										if(quietList >= quietLeft)begin
											listOutput <= 4'bxxxx;
										end
										else begin
											while(seats[quiet][listCount[quiet]] == 0)begin
												listCount[quiet] = listCount[quiet] + 1;
											end
											listOutput <= listCount[quiet];
											listCount[quiet] = listCount[quiet] + 1;
											quietList = quietList + 1;
										end
									end
									
									if(userID[5:4] == individual)begin
										numberOfInsideUser <= individualLeft;
										
										quietList = 0;
										loudList = 0;
										zoomList = 0;
										listCount[quiet] = 0;
										listCount[loud] = 0;
										listCount[zoom] = 0;
										
										if(individualList >= individualLeft)begin
											listOutput <= 4'bxxxx;
										end
										else begin
											while(seats[individual][listCount[individual]] == 0)begin
												listCount[individual] = listCount[individual] + 1;
											end
											listOutput <= listCount[individual];
											listCount[individual] = listCount[individual] + 1;
											individualList = individualList + 1;
										end
									end
									
									if(userID[5:4] == zoom)begin
										numberOfInsideUser <= zoomLeft;
										
										quietList = 0;
										individualList = 0;
										loudList = 0;
										listCount[quiet] = 0;
										listCount[individual] = 0;
										listCount[loud] = 0;
										
										if(zoomList >= zoomLeft)begin
											listOutput <= 4'bxxxx;
										end
										else begin
											while(seats[zoom][listCount[zoom]] == 0)begin
												listCount[zoom] = listCount[zoom] + 1;
											end
											listOutput <= listCount[zoom];
											listCount[zoom] = listCount[zoom] + 1;
											zoomList = zoomList + 1;
										end
									end
								end
							end

						endmodule