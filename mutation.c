//This function takes in an integer array mutated[] which represents the state of each cell in a lattice, 
//with a value of 1 indicating a normal cell and a value of 0 indicating a mutated cell. 
//The function takes in two integers: mutatedCount, which represents the number of cells that should be mutated, and totalcell, which represents the total number of cells in the lattice.
//The function uses the gsl_rng_uniform_int() function from the GSL library to generate a list of randomly mutated cells. 
//For each of the mutatedCount cells, a random integer between 0 and totalcell-1 is generated using gsl_rng_uniform_int(). The corresponding element in the mutated[] array is then set to 0, indicating that the cell is now mutated.

//In other words, this function randomly selects mutatedCount cells from the lattice and marks them as mutated by setting their values in the mutated[] array to 0.


void mutation(int mutated[], int mutatedCount, int totalcell, gsl_rng * r){
    int count = 0;  // counter for number of mutated cells
   
    while (count < mutatedCount) {
        int mutatedCell = gsl_rng_uniform_int(r, totalcell); // generate random index from remaining indices
        if (mutated[mutatedCell] == 1) { // check if cell has not already been mutated
            mutated[mutatedCell] = 0; // mutate cell
            count++; // increment counter
        }
       
    }
}
