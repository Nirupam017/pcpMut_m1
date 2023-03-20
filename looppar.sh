#!/bin/bash

#It is bash file. Ubuntu otherwise does not take read -a command

#Input file in csv format: first column job number.
# Job number is also used for output file names.
# Rest of the columns are parameter values viz m1, m2, m3 and sigma


# Record the starting time
start_time=$(date +%s.%N)


INPUT=input.csv


# Define IFS a ',' is not default delimiter
IFS=','

JOBS=0

# A loop to read each line and assign the line to an array
while read -a var
do 
	#Compile with elements read from the line
	gcc -Wall mainpcp_m1_mut.c -DFNVAL=\"${var[0]}\" -DM1VAL=${var[1]} -DM2VAL=${var[2]} -DM3VAL=${var[3]} -DM4VAL=${var[4]} -o ${var[0]} -lgsl -lgslcblas -lm -std=c99
	
	JOBS=$((JOBS+1))

done<$INPUT

echo "All $JOBS jobs are compiled!!"

echo "Jobs will be run using GNU Parallel!!"

parallel -j6 --progress ./{} ::: {1..10}

echo "All $JOBS jobs are executed!!"

# Record the ending time
end_time=$(date +%s.%N)

# Calculate the time taken in seconds
time_taken=$(echo "$end_time - $start_time" | bc)

# Print the time taken
echo "Time taken: $time_taken seconds"


