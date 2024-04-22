%% Message trasnmittier script
% The purpose of this script is to transmit the user inputs to each
% tethersonde
clear
clc

% Connect to the arduino 'Base Station'
reciever = serialport('COM17',115200);
% Each tethersonde after starting is expecting an array of inputs
% These inputs are declination and the Unix timestamp
% [Float, Long]
% These are parsed by the arduino but sent as 1 message
% A successful message is achieed when the tethersonde blinks purple
% After blining purple the tethersonde is recording data locally at 10 Hz
% and trasnmitting on its frequency the same data at 5 Hz, the limit of the
% arduino

% Send the declination then the time stamp last
declination = -10.85; % This is 10.85E

% a signal to start transmitting data
initialTimeStamp = datetime('now', 'TimeZone','UTC','Format', 'yyyy-MM-dd HH:mm:ss.SSSSSSS');

% Convert to a unix time stamp
timeAbsolute = posixtime(initialTimeStamp);

% The base station expects to read a string so we must formulate a string
% here
messageValues = [declination,timeAbsolute];
messageString = string(messageValues);
fullMessage = join(messageString, ','); % Comma delimiter

% send datums to the arduino to sendo over the LoRa
write(reciever,fullMessage,'uint8');

