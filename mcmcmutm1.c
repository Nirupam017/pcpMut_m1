//  This function performs the MCMC simulation steps
// This version uses the modified formulation for h3 the
// energy term for interaction with external field.
// Input: spin matrix, nbr matrix, dim
// Output: Change in energy due to swap; Spin matrix is updated accordingly



// A function to generate a random permutation of arr[]. Uses Fisher-Yates Algorithm
void randomize (int arr[], int n, gsl_rng * r);

// A utility function to swap two integers
void swap (int *a, int *b);

double mcmc(int **spin, double m1, double m2, double m3, int dim, int totalcell, int * mutated, int **nbr, gsl_rng * r){
     
    int selectCell; // Random number for selecting cell
    int selectEdge1; // Random number for selecting edge-1
    int selectEdge2; // Random number for selecting edge-2
    double u; //
    int edgeselect[6] = {0,1,2,3,4,5};

    double d[6][6] = {{0, -0.75, -1.5, -1.5, -0.75, 0},
                      {0.75, 0, -0.75, -0.75, 0, 0.75},
                      {1.5, 0.75, 0, 0, 0.75, 1.5},
                      {1.5, 0.75, 0, 0, 0.75, 1.5},
                      {0.75, 0, -0.75, -0.75, 0, 0.75},
                      {0, -0.75, -1.5, -1.5, -0.75, 0} };


    // Random numbers generated
    randomize(edgeselect, 6, r);
    selectEdge1 = edgeselect[0];
    selectEdge2 = edgeselect[1];
    selectCell = gsl_rng_uniform_int(r, totalcell);
    u = gsl_rng_uniform_pos(r);
    int nb_edge[6] = {3, 4, 5, 0, 1, 2}; // Array

	int nb1; // First neighbour of randomly selected cell
	int nb2; //Second neighbour for randomly selected cell

	// Internal alignment calculation
	nb1 = nbr[selectCell][selectEdge1];
	nb2 = nbr[selectCell][selectEdge2];
    
    int selectSpin1; // Spin of the first edge selected randomly
    int selectSpin2; // Spin of the second edge selected randomly
    
    selectSpin1 = spin[selectCell][selectEdge1];
    selectSpin2 = spin[selectCell][selectEdge2];
    
    if (selectSpin1 == selectSpin2){
        return 0.0;
    }
    else{
        
        int nbEdge1 = nb_edge[selectEdge1];; // Edge of the neighbour adjacent to first randomly selected edge of randomly selected cell
        int nbEdge2 = nb_edge[selectEdge2]; ; // Edge of the neighbour adjacent to second randomly selected edge of randomly selected cell
        
        int nbSpin1 = spin[nb1][nbEdge1]; // Spin of edge adjacent to first randomly selected edge of randomly selected cell
        int nbSpin2 = spin[nb2][nbEdge2]; // Spin of edge adjacent to first randomly selected edge of randomly selected cell
        
        double del_h1;
        double del_h2;
        double del_h3;
        
        del_h1 = m1*((selectSpin2 - selectSpin1)*(nbSpin1 - nbSpin2))*mutated[selectCell];
        
        int ScellSpin[6];
        ScellSpin[0] = spin[selectCell][0];
        ScellSpin[1] = spin[selectCell][1];
        ScellSpin[2] = spin[selectCell][2];
        ScellSpin[3] = spin[selectCell][3];
        ScellSpin[4] = spin[selectCell][4];
        ScellSpin[5] = spin[selectCell][5];
        
        int h2_old = 0;
        h2_old = ScellSpin[0]*ScellSpin[1] + ScellSpin[1]*ScellSpin[2] + ScellSpin[2]*ScellSpin[3] + ScellSpin[3]*ScellSpin[4] + ScellSpin[4]*ScellSpin[5] + ScellSpin[5]*ScellSpin[0];
        


        del_h3 = (selectSpin1 - selectSpin2)*d[selectEdge1][selectEdge2]*m3;

        
        // Swapping the spins to check
        swap(&ScellSpin[selectEdge1], &ScellSpin[selectEdge2]);

        
        int h2_new = 0;
        h2_new = ScellSpin[0]*ScellSpin[1] + ScellSpin[1]*ScellSpin[2] + ScellSpin[2]*ScellSpin[3] + ScellSpin[3]*ScellSpin[4] + ScellSpin[4]*ScellSpin[5] + ScellSpin[5]*ScellSpin[0];
        
 
        
        del_h2 = m2*(h2_new - h2_old);
        
        
        double del_h = (del_h1 + del_h2) + del_h3;
        
        if(u<=exp(-del_h)){
            // Swapping the spins permanently
            swap(&spin[selectCell][selectEdge1], &spin[selectCell][selectEdge2]);
        }

    }
    return 0;
}
// Utility functions
void randomize (int arr[], int n, gsl_rng * r){
    
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why a > 0
    for (int p = n-1; p > 0; p--)
    {
        // Pick a random index from 0 to a
        int q = gsl_rng_uniform_int(r, p+1);
        
        // Swap arr[i] with the element at random index
        swap(&arr[p], &arr[q]);
    }
}

void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

