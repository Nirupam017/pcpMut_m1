mainpcp_m1_mut.c:
Code now contains an additional parameter called percentMutated and its value will be given in the input file by the use. It represents percent of cells mutated in the total cells.
logfile now stores data about percentMutated in each simulation.
Code is edited to allow mcmcMutated function to take this percentMutated as a paraemeter.

mcmcmut_m1.c:
Code now stores an array of size total number of cells which stores either one or zero as each element. 1 implies cell is normal and zero implies it is mutated.
During calculation of h1 for a selected cell number, the whole term is multiplied with either 1 or 0 depending on its value in the above array to check whether a cell is mutated or not. In this way h1 is zero if the cell is mutated and h1 calculation is unaffected if the cell is normal.
