%% Tethersonde Reciever controller
% This script access the arduino base station through the serial port
% here we can collect and plot any data the is recieved by the arduino

clear
clc
%% connect to the arduino serial port
% Verify the port connection is correct by checking yourv arduino IDE first
% also check to see if any other serial monitors are open, they need to be
% closed.

reciever = serialport('COM17',115200);
timestamp = 1; % time in milliseconds

% Logging commands
duration = 0.01 * 3600; % 3 hour duration in seconds (1 h = 3600 s)
elapsedTime = 0; % initialize 1 seond, this is for matlab's timer
stream = []; % stream data array initilization
index = 1; % this is for indexing the proper rows of the array
% these index values are forvthe 6 potential sondes that could be recorded
% in this script
index1 = 1;
index2 = 1;
index3 = 1;
index4 = 1;
index5 = 1;
index6 = 1;

% Declare timer
startTime = tic;

% Declare the initial arrays and time stamps
% This is pre allocating for size and type

dataSet1 =  NaN(10, 11);
dataSet2 =  NaN(10, 11);
dataSet3 =  NaN(10, 11);
dataSet4 =  NaN(10, 11);
dataSet5 =  NaN(10, 11);
dataSet6 =  NaN(10, 11);
timeStampData1 = repmat(datetime('now', 'TimeZone','UTC','Format', 'yyyy-MM-dd HH:mm:ss.SSSSSSS'),1,10)';
timeStampData2 = repmat(datetime('now', 'TimeZone','UTC','Format', 'yyyy-MM-dd HH:mm:ss.SSSSSSS'),1,10)';
timeStampData3 = repmat(datetime('now', 'TimeZone','UTC','Format', 'yyyy-MM-dd HH:mm:ss.SSSSSSS'),1,10)';
timeStampData4 = repmat(datetime('now', 'TimeZone','UTC','Format', 'yyyy-MM-dd HH:mm:ss.SSSSSSS'),1,10)';
timeStampData5 = repmat(datetime('now', 'TimeZone','UTC','Format', 'yyyy-MM-dd HH:mm:ss.SSSSSSS'),1,10)';
timeStampData6 = repmat(datetime('now', 'TimeZone','UTC','Format', 'yyyy-MM-dd HH:mm:ss.SSSSSSS'),1,10)';


% Wind speed varibles
% These are the values form the calibration process
% Each sonde is transmitting a voltage value but for the plotting process
% we are calcualting velocity in real time here.
a = -0.4693;
b = 0.0147;
c = 0.5874;
d = 0.2344;

% This while loop runs for the declared duration and it will log anything
% that comes in over the serial monitor
while toc(startTime) < duration

    % Recieves data as a string
    % Uncomment if you want to see the string populate inthe Command Window
    stream = readline(reciever);

  
    % This is the timestamp in UTC the SSS increases the time resolution
    % for imperfect sampling
    % do not change the time format
    date = datetime('now', 'TimeZone','UTC','Format', 'yyyy-MM-dd HH:mm:ss.SSSSSSS');

    % This section takes the string data and splits it up
    % the strtok can only take the first datum
    % so we have to perfrom the breakdown for each set
    [identity,remaining] = strtok(stream, ', ');
    identity = str2double(identity);

    [azimuth,remaining] = strtok(remaining, ', ');
    azimuth = str2double(azimuth);

    [windVolt,remaining] = strtok(remaining, ', ');
    windVolt = str2double(windVolt);

    [tempSHT,remaining] = strtok(remaining, ', ');
    tempSHT = str2double(tempSHT);

    [humidSHT,remaining] = strtok(remaining, ', ');
    humidSHT = str2double(humidSHT);

    [altitude,remaining] = strtok(remaining, ', ');
    altitude = str2double(altitude);

    [pressure,remaining] = strtok(remaining, ', ');
    pressure = str2double(pressure);

    [tempThermo,remaining] = strtok(remaining, ', ');
    tempThermo = str2double(tempThermo);

    [battPercent,remaining] = strtok(remaining, ', ');
    battPercent = str2double(battPercent);

    % last statement doesn't need to include remaining
    % there ins't anything left remaining
    timestamp = strtok(remaining, ', ');
    timestamp = str2double(timestamp);

    % calculate wind speed with thermocouple temperature, currently with
    % Here we have set the temperature input to come form the SHT due to
    % wiring of the thermocouples not being complete during the tesiting of
    % the code
    windVolt = ((windVolt - a - b * tempSHT) / c) ^ (1/d);

    % Store data based on the identity
    % Each data set is also storing the Unix timestamp from matlab for easy
    % visual comparison; however, we are also storing a date time 
    % "Human readable" version as well
    if identity == 1
        dataSet1(index1, 1) = posixtime(date); % time stamp from matalb
        dataSet1(index1,2) = real(identity);
        dataSet1(index1,3) = real(azimuth);
        dataSet1(index1,4) = real(windVolt);
        dataSet1(index1,5) = real(tempSHT);
        dataSet1(index1,6) = real(humidSHT);
        dataSet1(index1,7) = real(altitude);
        dataSet1(index1,8) = real(pressure);
        dataSet1(index1,9) = real(tempThermo);
        dataSet1(index1,10) = real(battPercent);
        dataSet1(index1,11) = timestamp;
        timeStampData1(index1,1) = date;
        index1 = index1 + 1;


    elseif identity == 2
        dataSet2(index2, 1) = posixtime(date); % time stamp from matalb
        dataSet2(index2,2) = real(identity);
        dataSet2(index2,3) = real(azimuth);
        dataSet2(index2,4) = real(windVolt);
        dataSet2(index2,5) = real(tempSHT);
        dataSet2(index2,6) = real(humidSHT);
        dataSet2(index2,7) = real(altitude);
        dataSet2(index2,8) = real(pressure);
        dataSet2(index2,9) = real(tempThermo);
        dataSet2(index2,10) = real(battPercent);
        dataSet2(index2,11) = timestamp;
        timeStampData2(index2,1) = date;
        index2 = index2 + 1;



    elseif identity == 3
        dataSet3(index3, 1) = posixtime(date); % time stamp from matalb
        dataSet3(index3,2) = real(identity);
        dataSet3(index3,3) = real(azimuth);
        dataSet3(index3,4) = real(windVolt);
        dataSet3(index3,5) = real(tempSHT);
        dataSet3(index3,6) = real(humidSHT);
        dataSet3(index3,7) = real(altitude);
        dataSet3(index3,8) = real(pressure);
        dataSet3(index3,9) = real(tempThermo);
        dataSet3(index3,10) = real(battPercent);
        dataSet3(index3,11) = timestamp;
        timeStampData3(index3,1) = date;
        index3 = index3 + 1;

    elseif identity == 4
        dataSet4(index4, 1) = posixtime(date); % time stamp from matalb
        dataSet4(index4,2) = real(identity);
        dataSet4(index4,3) = real(azimuth);
        dataSet4(index4,4) = real(windVolt);
        dataSet4(index4,5) = real(tempSHT);
        dataSet4(index4,6) = real(humidSHT);
        dataSet4(index4,7) = real(altitude);
        dataSet4(index4,8) = real(pressure);
        dataSet4(index4,9) = real(tempThermo);
        dataSet4(index4,10) = real(battPercent);
        dataSet4(index4,11) = timestamp;
        timeStampData4(index4,1) = date;
        index4 = index4 + 1;


    elseif identity == 5
        dataSet5(index5, 1) = posixtime(date); % time stamp from matalb
        dataSet5(index5,2) = real(identity);
        dataSet5(index5,3) = real(azimuth);
        dataSet5(index5,4) = real(windVolt);
        dataSet5(index5,5) = real(tempSHT);
        dataSet5(index5,6) = real(humidSHT);
        dataSet5(index5,7) = real(altitude);
        dataSet5(index5,8) = real(pressure);
        dataSet5(index5,9) = real(tempThermo);
        dataSet5(index5,10) = real(battPercent);
        dataSet5(index5,11) = timestamp;
        timeStampData5(index5,1) = date;
        index5 = index5 + 1;



    elseif identity == 6
        dataSet6(index6, 1) = posixtime(date); % time stamp from matalb
        dataSet6(index6,2) = real(identity);
        dataSet6(index6,3) = real(azimuth);
        dataSet6(index6,4) = real(windVolt);
        dataSet6(index6,5) = real(tempSHT);
        dataSet6(index6,6) = real(humidSHT);
        dataSet6(index6,7) = real(altitude);
        dataSet6(index6,8) = real(pressure);
        dataSet6(index6,9) = real(tempThermo);
        dataSet6(index6,10) = real(battPercent);
        dataSet6(index6,11) = timestamp;
        timeStampData6(index6,1) = date;
        index6 = index6 + 1;

    end
    

    % Plotting code for real time profiles
    % There are only 3 plots currently but more can be included
    % Azimuth, temperature, and windspeed
    subplot(2, 2, 1);
    plot(timeStampData1(:,1), dataSet1(:,3), 'b');
    hold on
    plot(timeStampData2(:,1), dataSet2(:,3), 'r');
    hold on
    plot(timeStampData3(:,1), dataSet3(:,3), 'g');
    % hold on
    % plot(dataSet4(:,1), dataSet4(:,3), 'k');
    % hold on
    % plot(dataSet5(:,1), dataSet5(:,3), 'c');
    % hold on
    % plot(dataSet6(:,1), dataSet6(:,3), 'm');
    title('Azimuth');
    
    subplot(2, 2, 2);
    plot(timeStampData1(:,1), dataSet1(:,5), 'b');
    hold on
    plot(timeStampData2(:,1), dataSet2(:,5), 'r');
    hold on
    plot(timeStampData3(:,1), dataSet3(:,5), 'g');
    % hold on
    % plot(dataSet4(:,1), dataSet4(:,5), 'k');
    % hold on
    % plot(dataSet5(:,1), dataSet5(:,5), 'c');
    % hold on
    % plot(dataSet6(:,1), dataSet6(:,5), 'm');
    title('Temperature');


    subplot(2, 1, 2);
    plot(timeStampData1(:,1), dataSet1(:,4), 'b'); % No wind senor currently
    hold on
    plot(timeStampData2(:,1), dataSet2(:,4), 'r');
    hold on
    plot(timeStampData3(:,1), dataSet3(:,4), 'g');
    % hold on
    % plot(dataSet4(:,1), dataSet4(:,4), 'k');
    % hold on
    % plot(dataSet5(:,1), dataSet5(:,4), 'c');
    % hold on
    % plot(dataSet6(:,1), dataSet6(:,4), 'm');
    % title('Windspeed');
    legend("Sonde1", "Sonde2", "Sonde3");


    drawnow;

    index = index + 1; % Step up the index by one for each transmission recieved
end
% Afer the while loop ends each array of data that was saved is soved as a
% csv Timetable.

% Saving tethersonde data
% Sonde 1
% build a time table
matlabTime = timeStampData1;
matlabUnix = dataSet1(:,1);
identity = dataSet1(:,2);
azimuth = dataSet1(:,3);
windSpeed = dataSet1(:,4);
temp1 = dataSet1(:,5);
humidity = dataSet1(:,6);
altitude = dataSet1(:,7);
pressure = dataSet1(:,8);
temp2 = dataSet1(:,9);
battery = dataSet1(:,10);
sondeTime = dataSet1(:,11);
T1 = timetable(matlabTime,matlabUnix, identity, azimuth, windSpeed, temp1, humidity, altitude, pressure, temp2,battery, sondeTime);

% Save as a time table.
filename = 'sonde1.csv';
writetimetable(T1, filename);

% Sonde 2
% build a time table
matlabTime = timeStampData2;
matlabUnix = dataSet2(:,1);
identity = dataSet2(:,2);
azimuth = dataSet2(:,3);
windSpeed = dataSet2(:,4);
temp1 = dataSet2(:,5);
humidity = dataSet2(:,6);
altitude = dataSet2(:,7);
pressure = dataSet2(:,8);
temp2 = dataSet2(:,9);
battery = dataSet2(:,10);
sondeTime = dataSet2(:,11);
T2 = timetable(matlabTime,matlabUnix, identity, azimuth, windSpeed, temp1, humidity, altitude, pressure, temp2,battery, sondeTime);

% Save as a time table.
filename = 'sonde2.csv';
writetimetable(T2, filename);

% Sonde 3
% build a time table
matlabTime = timeStampData3;
matlabUnix = dataSet3(:,1);
identity = dataSet3(:,2);
azimuth = dataSet3(:,3);
windSpeed = dataSet3(:,4);
temp1 = dataSet3(:,5);
humidity = dataSet3(:,6);
altitude = dataSet3(:,7);
pressure = dataSet3(:,8);
temp2 = dataSet3(:,9);
battery = dataSet3(:,10);
sondeTime = dataSet3(:,11);
T3 = timetable(matlabTime,matlabUnix, identity, azimuth, windSpeed, temp1, humidity, altitude, pressure, temp2,battery, sondeTime);

% Save as a time table.
filename = 'sonde3.csv';
writetimetable(T3, filename);

% Averything below here is for sondes 4-6 which are currently not
% functional
% % Sonde 4
% % build a time table
% matlabTime = timeStampData4;
% matlabUnix = dataSet4(:,1);
% identity = dataSet4(:,2);
% azimuth = dataSet4(:,3);
% windSpeed = dataSet4(:,4);
% temp1 = dataSet4(:,5);
% humidity = dataSet4(:,6);
% altitude = dataSet4(:,7);
% pressure = dataSet4(:,8);
% temp2 = dataSet4(:,9);
% battery = dataSet4(:,10);
% sondeTime = dataSet4(:,11);
% T4 = timetable(matlabTime,matlabUnix, identity, azimuth, windSpeed, temp1, humidity, altitude, pressure, temp2,battery, sondeTime);
% 
% % Save as a time table.
% filename = 'sonde4.csv';
% writetimetable(T4, filename);
% 
% % Sonde 5
% % build a time table
% matlabTime = timeStampData5;
% matlabUnix = dataSet5(:,1);
% identity = dataSet5(:,2);
% azimuth = dataSet5(:,3);
% windSpeed = dataSet5(:,4);
% temp1 = dataSet5(:,5);
% humidity = dataSet5(:,6);
% altitude = dataSet5(:,7);
% pressure = dataSet5(:,8);
% temp2 = dataSet5(:,9);
% battery = dataSet5(:,10);
% sondeTime = dataSet5(:,11);
% T5 = timetable(matlabTime,matlabUnix, identity, azimuth, windSpeed, temp1, humidity, altitude, pressure, temp2,battery, sondeTime);
% 
% % Save as a time table.
% filename = 'sonde5.csv';
% writetimetable(T5, filename);
% 
% % Sonde 6
% % build a time table
% matlabTime = timeStampData6;
% matlabUnix = dataSet6(:,1);
% identity = dataSet6(:,2);
% azimuth = dataSet6(:,3);
% windSpeed = dataSet6(:,4);
% temp1 = dataSet6(:,5);
% humidity = dataSet6(:,6);
% altitude = dataSet6(:,7);
% pressure = dataSet6(:,8);
% temp2 = dataSet6(:,9);
% battery = dataSet6(:,10);
% sondeTime = dataSet6(:,11);
% T6 = timetable(matlabTime,matlabUnix, identity, azimuth, windSpeed, temp1, humidity, altitude, pressure, temp2,battery, sondeTime);
% 
% % Save as a time table.
% filename = 'sonde6.csv';
% writetimetable(T6, filename);

