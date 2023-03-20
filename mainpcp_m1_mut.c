/* The main file for PCP simulation
Primarily based on the work Kamleswar & Biplab 
Phys Rev E . 2019 Sep;100(3-1):032408.
Originally created by Himakar Sreerangam on 20/10/19.
Modified by Biplab Bose and Nirupam 2021 */


// Generic library functions
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h> 
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <gsl/gsl_rng.h> 
#include <gsl/gsl_statistics.h>

// Custom functions for PCP
#include "seedSpin.c"
#include "nebor.c"
#include "mcmcmutm1.c"
#include "chkalign.c"
#include "cluster.c"
#include "mutation.c"





// A utility function random seed
unsigned long int random_seed();

// A utility function to calculate energy 
double energy_calc(int **nbr, int **spin, int totalcell, double m1, double m2, double m3);


//**** Input parameter values for the model *** //

// Interaction parameters
double m1 = M1VAL;  
double m2 = M2VAL;  
double m3 = M3VAL;
double percentMutated = M4VAL;

   

// dimension of lattice
int dim = 32;



// Intitial spin configuration. 
// 0 = all cells identically completly algined in any of the six directions; 
// 1 = random; 
// 2 = all cells completely aligned in 0-direction

int randSpin = 1;


// Number of MC steps per cell
int N = 52000;

// Number of burning steps per cell
int B = 2000;

// Sampling intervals per cell
int SI = 100;

//Name of output files
char FN[] = FNVAL;


//********** Parameter values ends ************//

//Extern variable shared by functions
//extern unsigned long totalcell; 


//**** Main function starts *****//

int main(){

    //Timer
    double time_spent = 0.0; 
    clock_t begin= clock();
    
    //Random seed
    unsigned long int mySeed; 
    mySeed = random_seed();
    

    //Total number of mcmc-steps
    long long mcmcSteps = (dim*dim)*(long long)N;

   //Burning steps
    long long burningPhase = (dim*dim)*(long long)B; 

    // Sampling intervals
    long long sampleInt = (dim*dim)*(long long)SI;

    //Total number of cells
    int totalcell = dim*dim; 

       
    
    // Random number generation set-up
    const gsl_rng_type * T;
    gsl_rng * r;
    gsl_rng_env_setup();
    T = gsl_rng_ranlxs0;
    r = gsl_rng_alloc (T);
    gsl_rng_set(r, mySeed);
   
    
    //Creating spin, nbr matrices
    int **spin; 
    int **nbr; 
    spin = seedSpin(dim, randSpin, mySeed, r); 
    nbr = nebor(dim); 
    int mutatedCount = totalcell * percentMutated;  //count of mutated cells 
    int mutated[totalcell]; 
    for (int i = 0; i < totalcell; ++i)
    {
        mutated[i] = 1;
    }

    mutation(mutated, mutatedCount, totalcell, r);
    
   
    
    //---- MCMC Starts ----//

    int n_sample = (mcmcSteps-burningPhase)/(sampleInt);
    long long mc_step = 0;

    //MCMC in burning phase
    for(mc_step = 1; mc_step<=burningPhase; mc_step++){
        mcmc(spin, m1, m2, m3, dim, totalcell, mutated, nbr, r);
    }

    //Defines arrays & variables for data 
    double align[4] = {0};

    int *upcell = (int*)malloc(sizeof(int)*totalcell);

    int **alignMatrix = (int**)malloc(sizeof(int*)*totalcell);
    for (int j = totalcell; j--; ){
        alignMatrix[j] =  (int*)malloc(sizeof(int)*4);
    }

        
    int **all_cs = (int**)malloc(sizeof(int*)*totalcell);
    for (int j = totalcell; j--; ){
        all_cs[j] =  (int*)malloc(sizeof(int)*n_sample);
    }


    int nc = 0;
    

    // Sum(S) and sum-squares (SS) for Energy per cell (E), 
    // fraction of internal aligned (IA), fraction External aligned (EA),
    // fraction of both internal external aligned (AA),
    // fraction aligned up (AU), Percolation strength

    
    double IA_S = 0;  
    double EA_S = 0; 
    double AA_S = 0; 
    double AU_S = 0; 
    double P_S = 0; 
    long long NC_S = 0;
  
    double IA_SS = 0; 
    double EA_SS = 0; 
    double AA_SS = 0; 
    double AU_SS = 0; 
    double P_SS = 0;
    long long NC_SS = 0;

 
    int sample_num = 0;

    //MCMC in post burning phase
    for(mc_step=burningPhase + 1; mc_step<=mcmcSteps; mc_step++){
        
        mcmc(spin, m1, m2, m3, dim, totalcell, mutated, nbr, r);

        

        //Collect data
        if(mc_step%sampleInt == 0){

            
            chkalign(align, spin, nbr, totalcell, upcell, alignMatrix);

            IA_S = IA_S + align[0];
            EA_S = EA_S + align[1];
            AA_S = AA_S + align[2];
            AU_S = AU_S + align[3];
            IA_SS = IA_SS + align[0]*align[0];
            EA_SS = EA_SS + align[1]*align[1];
            AA_SS = AA_SS + align[2]*align[2];
            AU_SS = AU_SS + align[3]*align[3];

            //Cluster data
            double P = 0;
            int *cs;

            cs = cluster(&P, &nc, upcell, nbr, dim, totalcell);

            P_S = P_S + P;
            P_SS = P_SS + P*P;
            NC_S = NC_S + nc;
            NC_SS = NC_SS + nc*nc;



            for(int a=totalcell; a--; ){
                all_cs[a][sample_num] = cs[a];
            }

            

            ++sample_num;
            free(cs);
            
       
        }
              
    }

    // MCMC over //



        //Calculate  S and its variance

    
        double *mean_cs = calloc(totalcell, sizeof(mean_cs));
        double *var_cs = calloc(totalcell, sizeof(var_cs));
        double var_sscs = 0;
        double var_scs = 0;
        double sum_sscs = 0;
        double sum_scs = 0;

        for(int k = 0; k<totalcell; k++){

            //Take cs data of a particular size
            double acs[n_sample];
            for(int m=0; m<n_sample;m++){
                acs[m]=all_cs[k][m];
            }

            //Get mean & var of cs data of a particular size
            mean_cs[k] = gsl_stats_mean(acs,1,n_sample);
            var_cs[k] = gsl_stats_variance_m(acs,1,n_sample,mean_cs[k]);

            int sz = k+1; //cluster size

            //Calculate sums in denominator & numerators
            sum_sscs = sum_sscs+ sz*sz*mean_cs[k];
            sum_scs = sum_scs + sz*mean_cs[k];


            //Calculate variances of denominator and numerator.
            var_sscs = var_sscs + sz*sz*sz*sz*var_cs[k];
            var_scs = var_scs + sz*sz*var_cs[k];
           
            
        }

        double S = 0;
        double var_S = 0;
        if(sum_scs>0){
            S = sum_sscs/sum_scs;
            var_S = S*S*(var_sscs/(sum_sscs*sum_sscs) + var_scs/(sum_scs*sum_scs));
        }        


       
      
    

    //-------------Output files------------------------//
    
    //create file names
    char FN2[1000];
    strcpy(FN2,FN);

    char *output1;
    output1 = strcat(FN, "_data");
    output1 = strcat(output1, ".csv");

    FILE *fp1; 
    fp1 = fopen(output1, "w");
    
    
    fprintf(fp1, "n_sample, IA_S, IA_SS, EA_S, EA_SS, AA_S, AA_SS, \
        AU_S, AU_SS, P_S, P_SS, NC_S, NC_SS, S, var_S\n");
    fprintf(fp1, "%d, %.15g, %.15g, %.15g, %.15g, %.15g, %.15g, %.15g, %.15g, %.15g, %.15g, %lld, \
        %lld, %.15g, %.15g\n",n_sample,IA_S,IA_SS,\
        EA_S,EA_SS,AA_S,AA_SS,AU_S,AU_SS,P_S, P_SS, NC_S, NC_SS,S, var_S);
    
       
    fclose(fp1);
    
     
    char *output2;
    output2 = strcat(FN2, "_log");
    output2 = strcat(output2, ".txt");

    FILE *fp2; 
    fp2 = fopen(output2, "w");
    
    fprintf(fp2, "dim = %d\n", dim);
    fprintf(fp2, "randspin = %d\n", randSpin);
    fprintf(fp2, "m1 = %f\n", (m1));
    fprintf(fp2, "m2 = %f\n", (m2));
    fprintf(fp2, "m3 = %f\n", m3);
    fprintf(fp2, "percent cells Mutated = %f\n", percentMutated);
    fprintf(fp2, "Total Number of MC steps = %lld\n", mcmcSteps);
    fprintf(fp2, "Total Number of burning steps = %lld\n", burningPhase);
    fprintf(fp2, "Sampling interval = %lld\n", sampleInt);
    fprintf(fp2, "data samples = %d\n", n_sample);

    clock_t end = clock();
    time_spent += (double)(end-begin)/CLOCKS_PER_SEC;

    fprintf(fp2, "Total time spent (sec): %f\n", time_spent);

    fclose(fp2);



       
    //--------------Output files ends-------------//


    // -- free ---
    gsl_rng_free (r); 
    
    free(upcell);
    
    
    for (int i=0; i<totalcell; i++){
            free(nbr[i]);
    }
    free(nbr);

    for (int i=0; i<totalcell; i++){
            free(spin[i]);
    }
    free(spin);


    for (int i = totalcell; i--; ){
        free(alignMatrix[i]);
    }
    free(alignMatrix);

    for (int i = totalcell; i--; ){
        free(all_cs[i]);
    }
    free(all_cs);

    free(mean_cs);
    free(var_cs);

    // -- all freed ----

    
    printf("MCMC Simulation is over\n");
    return 0;
}

//******* Main function ends ******//




//****** Utility functions ****** //

unsigned long int random_seed(){
    // To generate a unique seed for random number generator
    struct timeval tv;
    gettimeofday(&tv,0);
    return (tv.tv_sec + tv.tv_usec);
    
}

double energy_calc(int **nbr, int **spin, int totalcell, double m1, double m2, double m3){
    double energy = 0.0;
    double h2 = 0;
    double h1 = 0;
    double h3 = 0;
    
    for (int i=0; i<totalcell; i++){
        h2 = h2 + m2*(spin[i][0]*spin[i][1]+ spin[i][1]*spin[i][2]+ spin[i][2]*spin[i][3]+ spin[i][3]*spin[i][4]+ spin[i][4]*spin[i][5]+ spin[i][5]*spin[i][0]);
        
        for (int j =0; j<6; j++){
            h1 = h1 + m1*(spin[nbr[i][j]][(j+3)%6]*spin[i][j]);
        }
        
        h3 = h3 +m3*(spin[i][0]+spin[i][5]);
    }
    
    energy = h1/2.0 + h2 + h3;
    return energy;
}
