% read the data from the csv file
data = readtable('dataCalculated.csv');

% extract unique m1 values
m1_values = unique(data.m1);

% sort the m3 values in ascending order
sorted_m3 = sort(unique(data.m3),'descend');

% create empty matrices to hold the UA, CA and P values
UA_matrix = zeros(length(sorted_m3), length(m1_values));
CA_matrix = zeros(length(sorted_m3), length(m1_values));
P_matrix = zeros(length(sorted_m3), length(m1_values));

% fill the matrices with the UA, CA and P values
for i = 1:length(m1_values)
    current_m1 = m1_values(i);
    current_data = data(data.m1 == current_m1, :);
    current_UA = current_data.UA;
    current_CA = current_data.CA;
    current_P = current_data.P;
    current_m3 = current_data.m3;
    [~, index] = ismember(current_m3, sorted_m3);
    UA_matrix(index, i) = current_UA;
    CA_matrix(index, i) = current_CA;
    P_matrix(index, i) = current_P;
end

% print the matrices as csv files
writematrix(UA_matrix, 'FU.csv');
writematrix(CA_matrix, 'FA.csv');
writematrix(P_matrix, 'P.csv');
