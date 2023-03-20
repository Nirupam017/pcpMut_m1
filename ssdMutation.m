% Read in the data files

data_0 = readmatrix('data_0_mut.csv');
data_5 = readmatrix('data_5_mut.csv');
data_10 = readmatrix('data_10_mut.csv');

% Extract the unique values of m1 and m3
m1 = unique(data_0(:,1));
m3 = unique(data_0(:,4));

% Create tables to store the results
results_table_upward = table(m1, zeros(size(m1)), zeros(size(m1)), 'VariableNames', {'m1', 'ssd_0_5_upward', 'ssd_0_10_upward'});
results_table_CA = table(m1, zeros(size(m1)), zeros(size(m1)), 'VariableNames', {'m1', 'ssd_0_5_CA', 'ssd_0_10_CA'});
results_table_P = table(m1, zeros(size(m1)), zeros(size(m1)), 'VariableNames', {'m1', 'ssd_0_5_P', 'ssd_0_10_P'});

% Calculate the SSDs between 0% and 5%, 0% and 10% at each unique value of m1 for each property
for i = 1:length(m1)
    % Find the rows in each data set that correspond to the current value of m1
    rows_0 = data_0(:,1) == m1(i);
    rows_5 = data_5(:,1) == m1(i);
    rows_10 = data_10(:,1) == m1(i);
    
    % Extract the relevant data for the current value of m1
    data_0_m1 = data_0(rows_0, :);
    data_5_m1 = data_5(rows_5, :);
    data_10_m1 = data_10(rows_10, :);
    
    % Calculate the SSD between 0% and 5% at the current value of m1 for each property
    ssd_0_5_upward_m1 = sum((data_0_m1(:,9) - data_5_m1(:,9)).^2);
    ssd_0_5_CA_m1 = sum((data_0_m1(:,11) - data_5_m1(:,11)).^2);
    ssd_0_5_P_m1 = sum((data_0_m1(:,13) - data_5_m1(:,13)).^2);
    
    % Calculate the SSD between 0% and 10% at the current value of m1 for each property
    ssd_0_10_upward_m1 = sum((data_0_m1(:,9) - data_10_m1(:,9)).^2);
    ssd_0_10_CA_m1 = sum((data_0_m1(:,11) - data_10_m1(:,11)).^2);
    ssd_0_10_P_m1 = sum((data_0_m1(:,13) - data_10_m1(:,13)).^2);
    
    % Store the results in the corresponding tables
    results_table_upward.ssd_0_5_upward(i) = ssd_0_5_upward_m1;
    results_table_upward.ssd_0_10_upward(i) = ssd_0_10_upward_m1;
    
    results_table_CA.ssd_0_5_CA(i) = ssd_0_5_CA_m1;
    results_table_CA.ssd_0_10_CA(i) = ssd_0_10_CA_m1;
    

    results_table_P.ssd_0_5_P(i) = ssd_0_5_P_m1;
    results_table_P.ssd_0_10_P(i) = ssd_0_10_P_m1;
end

    % Write the results tables to CSV files
    writetable(results_table_upward, 'ssd_UA.csv');
    writetable(results_table_CA, 'ssd_CA.csv');
    writetable(results_table_P, 'ssd_P.csv');


