 % Initiate serial port for the MCU
 
%UART data buf {PositionByte, Motor Selection, Direction, 4 Steps Byte}
% 0x60 - Motor x
% 0x70 - Motor Y
% 0x20 - Direction Left
% 0x40 - Direction Right
% 1 cm = 2142 steps (decimal)
 clear;
 file_name ='Pattern_t.xlsx';
 MCU_port = serialport("COM10",115200,"DataBits",8, "StopBits",1, "Parity","none");
 specAnalyzer = specAnalyzer_init();
 %MCU_port.Timeout = 10;
 
 MxLeft     = uint8([0x00, 0x60, 0x20, 0x00, 0x00, 0x29, 0xDA]);     % Motor X   
 MxRight    = uint8([0x00, 0x60, 0x40, 0x00, 0x00, 0x29, 0xDA]);
 
 MyLeft     = uint8([0x00, 0x70, 0x20, 0x00, 0x00, 0x29, 0xDA]);     % Motor Y
 MyRight    = uint8([0x00, 0x70, 0x40, 0x00, 0x00, 0x29, 0xDA]);

 MyLeftDx     = uint8([0x00, 0x70, 0x20, 0x00, 0x00, 0x08, 0x5E]);   %to start the measurement
 MyRightDx    = uint8([0x00, 0x70, 0x40, 0x00, 0x00, 0x08, 0x5E]);
 SlideReturnZero = uint8([0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]);
 
 Flag_My = 0xFF;
 Flag_Mx = 0xAA;
 TestCount= 20;
 Dx = 17;           %Samples No of X
 Dy = 17;           %Samples No of Y
 data_buf = zeros(Dx,Dy);
 delaySeconds = 2; %must >=0.08 ,, better to >=0.1
 pause(delaySeconds);

 %data_buf(1,1)=read_Data(specAnalyzer);
 
 for i = 1:Dx

    % Determine the direction of Y motor movement based on whether i is odd or even
    if mod(i, 2) == 1
        yDirection = MyLeft;  % Move Y motor from 0 to max (left)
        yIndexRange = 1:Dy;    % Forward indexing
    else
        yDirection = MyRight;  % Move Y motor from max to 0 (right)
        yIndexRange = Dy:-1:1; % Reverse indexing
    end

    % Move Y motor across its range
    for j = yIndexRange
        data_buf(i,j) = read_Data(specAnalyzer);

        % If at the final position in Y direction, break out of the loop
        if j == yIndexRange(end)
            break;
        end

        % Move Y motor and wait for it to reach the position
        write(MCU_port, yDirection, "uint8");
        pause(delaySeconds);

        % Check if the Y motor has reached the desired position
        returnFlagY = read(MCU_port, 1, "uint8");
        if returnFlagY ~= Flag_My
            break;  % Exit the loop if the Y motor doesn't reach the expected position
        end
    end

    % Move X motor to the next position and wait
    write(MCU_port, MxLeft, "uint8");
    pause(delaySeconds);

    % Check if the X motor has reached the desired position
    returnFlagX = read(MCU_port, 1, "uint8");
    if returnFlagX ~= Flag_Mx
        break;  % Exit the loop if the X motor doesn't reach the expected position
    end
end

write(MCU_port, SlideReturnZero, "uint8"); 
pause(4);

 data_table = array2table(data_buf);
 writetable(data_table,  file_name);