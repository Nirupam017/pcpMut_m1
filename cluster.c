//Function that identify clusters of cells 
//that are aligned UP.
//This function takes the vector upcell that list
//cell that are aloigned UP and not. UP = 1 & Not UP = 0.
//This function calculates & updates variable P = percolation strength
            
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


//Utiliti function list
void countClusters(int M[], int *visited, int cluster_size[], int totalcell, int **nbr, int *stack);

void DFS(int M[], int n_cell, int *visited, int count, int cluster_size[], int **nbr, int *stack);

int isSafe(int M[], int n_cell, int *visited);

void push(int item, int *stack);

int pop(int *stack);

int peek(int *stack);

bool isStackEmpty();


int top = -1;


int *cluster(double *P, int *nc, int *upcell, int **nbr, int dim, int totalcell){

    int stack[totalcell];
   
    int *visited;
    visited = calloc(totalcell, sizeof(int));

    
    int *cluster_size = (int*)calloc(totalcell, sizeof(int));

    countClusters(upcell, visited, cluster_size, totalcell, nbr, stack); // Obtaining the size of every cluster in the lattice. Smallest cluster size = 1 (1 cell)
                
    int n_clusters = 0;
    
    int bigg = 0;

    int *cs = (int*)calloc(totalcell, sizeof(int));

    for(int a=0; a<totalcell; a++){
        if(cluster_size[a] != 0){

            //count total number of cluster
            ++n_clusters;

            //find biggest cluster
            bigg = MAX(bigg, cluster_size[a]);


            //fill cs data
            cs[cluster_size[a] - 1] = cs[cluster_size[a] - 1] + 1;

        }
        else{
            break;
        }
    }

    //Get percolation strength
    *P = (double)bigg/totalcell;


    
    

    //Get number of clusters
    *nc = n_clusters;
    



    free(cluster_size);
    free(visited);

    //Make the count of biggest cluster zero and retrun cs
    cs[bigg-1] = 0;
    return cs;

}


// ********* Utility Functions Starts ***********//

//Utility function to return an array containing clusters of different sizes
void countClusters(int M[], int *visited, int cluster_size[], int totalcell, int **nbr, int *stack){
    
    // Initialize count as 0 and travese through the all cells of
    
    
    int count = 0;
    for (int i=totalcell; i--; ){
        if (M[i] && !visited[i]){ // If a cell with value 1 is not // visited yet, then new cluster found
            ++cluster_size[count];
            DFS(M, i, visited, count, cluster_size, nbr, stack); // Visit all cells in this cluster.
            ++count; // and increment cluster count
        }
    }
}

//Utility function to perform DFS for our nodes and edges
void DFS(int M[], int n_cell, int *visited, int count, int cluster_size[], int **nbr, int *stack){

    // Mark this cell as visited
    visited[n_cell] = 1;
    
    push(n_cell, stack);

    while(!isStackEmpty()){
        
        n_cell = peek(stack);
        pop(stack);
        
        // Recur for all connected neighbours
        for (int k = 6; k--; ){
            if (isSafe(M, nbr[n_cell][k], visited)){
                ++cluster_size[count];
                visited[nbr[n_cell][k]] = 1;
                push(nbr[n_cell][k], stack);
            }
        }
    }
}

//Utility function to check if cell is 0-aligned, i.e., M[i] == 1 and not visited
int isSafe(int M[], int n_cell, int *visited){
// and not yet visited
    return (M[n_cell] && !visited[n_cell]);
}


//stack functions
void push(int item, int *stack) {
   stack[++top] = item;
}

int pop(int *stack) {
   return stack[top--];
}

int peek(int *stack) {
   return stack[top];
}

bool isStackEmpty() {
   return top == -1;
}
