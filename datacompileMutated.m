
% Code to read data from multiple data files of simulation.
% The file architecture is: (a) 'input.csv' having parameter values and simulation number,
% (b) '*_data.csv' having data.
% input.csv has: 1st column - simulation number, 2nd column- m1 value,
% 3rd column - m2 value, 4th column - m3 value and 5th column - % of mutated cells. 
% If required other parameters can be added in subsequent columns.
% *_data.csv: * = simulation number, taken from input.csv file. Stores measured quantities.
% For most measured quantities, sum of squares and sum of values are stored. For S, S and variance
% of S is stored. Check the sequence of quantities in the input file. 
% 
% OUTPUT: The csv out put file has m1, m2, m3, sigma as first four columns. Rest of the columns
% have means and stadrad deviations of different measurments.
% The sequence of calculated variable is same as in the data file.
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CAUTION: Some part of this code has hard coding as we can not fix the number of variables
% in the data file.
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



% Clear all variables
clear


% Read input.csv
input1 = readmatrix('input.csv');

% Number of simulations
numSim = size(input1,1);

% Number of variables in each data files
input2 = readmatrix('1_data.csv');
numPara = size(input2,2);

% Create a bigger data matrix
dataAll(numSim, numPara) = 0;


% read data from each file
for j = 1:numSim
    fileName = sprintf('%d_data.csv', j);
    dataAll(j,:)= readmatrix(fileName);
end

% Create matrix for calculated data
dataCal(numSim, numPara-1) = 0;

% Calculate means and std dev
% CAUTION: Hardcoding for variables in loop
for j = 1:2:11
    dataCal(:,j) = dataAll(:,j+1)./dataAll(:,1);
    dataCal(:,j+1) = sqrt(dataAll(:,j+2)./dataAll(:,1) - dataCal(:,j).^2);
end

% S and std error of S
% CAUTION: Hardcoding for variables in array
dataCal(:,13) = dataAll(:,14);
dataCal(:,14) = sqrt(dataAll(:,15)./dataAll(:,1));

% Concatenate input parameter values with calculated values
dataCal = [input1(:,2:5) dataCal];

% Convert the matrix to a table containing column titles and print it.
T = array2table(dataCal,...
    'VariableNames',{'m1','m2','m3','mutated%','IA','IA_SD','EA','EA_SD','CA','CA_SD','UA','UA_SD','P','P_SD','NC_S','NC_SS','S','S_VAR'});

writetable(T,'dataCalculated.csv');



