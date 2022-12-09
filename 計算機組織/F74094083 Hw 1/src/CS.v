`timescale 1ns/10ps

module CS(Y, X, reset, clk);

input clk, reset;
input [7:0] X;
output reg [9:0] Y;//remember to set reg

reg[7:0] temp [0:8];
integer i;
reg [10:0] sum;
reg  [11:0] max;

always@(posedge reset or posedge clk)
begin
    if(reset)
    begin
        Y <= 10'd0;
        max <= 0;
        sum <=0;
        i <= 0;
        for(i = 0; i<9; i=i+1)
            temp[i] = 8'd0;
    end
    else
    begin
        for(i=0; i < 8; i=i+1)
            temp[i] <= temp[i+1];

        temp[8] <= X;
        sum <= sum+ X-temp[0];
    end
end
always@(*)
begin
    max=0;
    for(i=0; i<9; i=i+1)
        if((sum/9>=temp[i]) &&(temp[i]> max) )
            max = temp[i];

    Y <= (sum + max * 9) / 8;
end
endmodule