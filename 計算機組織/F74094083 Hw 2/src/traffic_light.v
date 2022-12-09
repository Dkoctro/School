module traffic_light (clk, rst, pass, R, G, Y);

input clk, rst, pass;
output R, G, Y;
reg R, G, Y;
reg [5:0] count;
reg [4:0] state;
reg [13:0] CLKNUM;

always@(posedge rst or posedge clk)
begin
    if(rst)
    begin
        G <= 1'b1;
        R <= 1'b0;
        Y <= 1'b0;
        count <= 6'b0;
        state <= 5'd0;
        CLKNUM<= 0;
    end
    else
    begin 
        CLKNUM <= CLKNUM+1;
        if(pass)
        begin 
            if(state >= 5'd7)
            begin        
                G <= 1'b1;
                R <= 1'b0;
                Y <= 1'b0;
                count <= 6'b0;
                state <= 5'd0;
            end
            else
            begin
                G <= G;
                R<= R;
                Y <= Y;
                count <= count+1;
                state <= state;
            end
        end
        else
        begin
            if(count == 6'b111111)
            begin
                if(state == 5'd7)
                begin
                    state <= state + 5'd1;
                    G <= 1'b0;
                    R <= 1'b0;
                    Y <= 1'b0;
                end
                else if(state == 5'd8)
                begin
                    state <= state + 5'd1;
                    G <= 1'b1;
                    R <= 1'b0;
                    Y <= 1'b0;
                end
                else if(state == 5'd9)
                begin
                    state <= state + 5'd1;
                    G <= 1'b0;
                    R <= 1'b0;
                    Y <= 1'b0;
                end
                else if(state == 5'd10)
                begin
                    state <= state + 5'd1;
                    G <= 1'b1;
                    R <= 1'b0;
                    Y <= 1'b0;
                end
                else if(state == 5'd11)
                begin
                    state <= state + 5'd1;
                    G <= 1'b0;
                    R <= 1'b0;
                    Y <= 1'b1;
                end
                else if(state == 5'd15)
                begin
                    state <= state + 5'd1;
                    G <= 1'b0;
                    R <= 1'b1;
                    Y <= 1'b0;
                end
                else if(state == 5'd23)
                begin
                    state <= 5'd0;
                    G <= 1'b1;
                    R <= 1'b0;
                    Y <= 1'b0;
                end
                else    
                begin
                    state <= state+1;
                    R <= R;
                    G <= G;
                    Y <= Y;
                end
                count <= 0;
            end
            else
            begin
                count <= count + 6'b1;
                state <= state;
                R <= R;
                G <= G;
                Y <= Y;
            end
               
        end
    end
end


endmodule
