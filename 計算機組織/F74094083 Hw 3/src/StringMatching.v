module SME(clk,reset,chardata,isstring,ispattern,valid,match,match_index);

input clk;
input reset;
input [7:0] chardata;
input isstring;
input ispattern;

output match;
output [4:0] match_index;
output valid;
reg match;
reg [4:0] match_index;
reg valid;

integer i;
reg [6:0] index;
reg [4:0] index2;

reg [7:0] storage [0:31];
reg [7:0] pattern [0:7];

reg head; // the head symbol
reg tail; // the tail symbol
reg clear_string; // reset sring
reg clear_pattern; // reset pattern

reg [6:0] string_length; // the length of string
reg [4:0] pattern_length; // the length of pattern

reg pass; // check if the pattern is correspond to the string
reg init;

reg head_temp;
reg pair;

always@(posedge clk or posedge reset)
begin
    if(reset)
    begin
        for(i = 0; i < 32; i = i + 1)
            storage[i] <= 8'd0;
        for(i = 0; i < 8; i = i + 1)
            pattern[i] <= 8'd0;

        index <= 6'd0;
        index2 <= 4'd0;

        valid <= 1'd0;
        match <= 1'd0;
        match_index <= 5'd0;

        string_length <= 6'd0;
        pattern_length <= 4'd0;

        clear_pattern <= 1'd0;
        clear_string <= 1'd0;

        head <= 1'd0;
        tail <= 1'd0;
        pass <= 1'd0;
        init <= 1'd1;
        pair <= 1'd0;

        head_temp <= 1'd0;
    end
    else
    begin
        if(isstring)
        begin
            valid <= 1'd0;
            match <= 1'd0;            
            if(!clear_string)
            begin
                for(i = 1; i < 32; i =  i + 1)
                    storage[i] <= 8'd0;
                storage[0] <= chardata;
                string_length <= 7'd1;
                clear_string <= 1'd1;
            end
            else
            begin
                storage[string_length] <= chardata;
                string_length <= string_length + 1;
            end
        end
        else if(ispattern)
        begin
            valid <= 1'd0;
            match <= 1'd0;
            if(chardata == 36)
                tail <= 1'd1;
            else if(chardata == 94)
                head <= 1'd1;
            else
                if(!clear_pattern)
                begin

                    for(i = 1; i < 8; i = i + 1)
                        pattern[i] <= 8'd0;
                    pattern[0] <= chardata;
                    pattern_length <= 5'd1;
                    init <= 1'd1;
                    clear_pattern <= 1'd1;
                end
                else
                begin
                    pattern[pattern_length] <= chardata;
                    pattern_length <= pattern_length + 1;
                end
        end
        else
        begin
            if(init)
            begin
                index <= 6'd0;
                index2 <= 4'd0;
                init <= 1'd0;
            end
            else
            begin
                if(index == string_length && (!pass || index2 != pattern_length))
                begin
                    valid <= 1'd1;
                    match <= 1'd0;
                    clear_pattern <= 1'd0;
                    clear_string <= 1'd0;
                    head <= 1'd0;
                    tail <= 1'd0;
                    head_temp <= 1'd0;
                end

                else if(index2 == pattern_length)
                begin
                    if(tail)
                    begin
                        if(index == string_length)
                        begin
                            pass <= 1'd1;
                            valid <= 1'd1;
                            match <= 1'd1;
                            match_index <= index - index2;
                            clear_pattern <= 1'd0;
                            clear_string <= 1'd0;
                            head <= 1'd0;
                            tail <= 1'd0;
                            head_temp <= 1'd0;
                        end
                        else if(storage[index] == 8'd32)
                        begin
                            pass <= 1'd1;
                            valid <= 1'd1;
                            match <= 1'd1;
                            match_index <= index - index2;
                            clear_pattern <= 1'd0;
                            clear_string <= 1'd0;
                            head <= 1'd0;
                            tail <= 1'd0;
                            head_temp <= 1'd0;
                        end
                        else
                        begin
                            pass <= 1'd0;
                            index <= index + 1;
                            index2 <= 4'd0;
                            if(head_temp)
                            begin
                                head <= 1'd1;                    
                                head_temp <= 1'd0;
                            end
                        end
                    end
                    else
                    begin
                        pass <= 1'd1;
                        valid <= 1'd1;
                        match <= 1'd1;
                        match_index <= index - index2;
                        clear_pattern <= 1'd0;
                        clear_string <= 1'd0;
                        head <= 1'd0;
                        tail <= 1'd0;
                        head_temp <= 1'd0;
                    end
                end

                else if(storage[index] == pattern[index2] || pattern[index2] == 8'd46)
                begin
                    if(head && storage[index-1] == 8'd32)
                    begin
                        index <= index + 1;
                        index2 <= index2 + 1;
                        pass <= 1'd1;
                        head <= 1'd0;
                        head_temp <= 1'd1;
                        pair <= 1'd1;
                    end
                    else if(!head)
                    begin
                        index <= index + 1;
                        index2 <= index2 + 1;
                        pass <= 1'd1;
                        pair <= 1'd1;
                    end
                    else
                    begin
                        index <= index + 1;
                        index2 <= 4'd0;
                        pass <= 1'd0;
                    end
                end
                else
                begin                    
                    if(pair)
                    begin
                        index <= index - index2 + 1;
                        pair <= 1'd0;
                    end
                    else
                        index <= index + 1;
                    index2 <= 4'd0;
                    pass <= 1'd0;
                    if(head_temp)
                    begin
                        head <= 1'd1;                    
                        head_temp <= 1'd0;
                    end
                end
            end
        end
    end
end


endmodule