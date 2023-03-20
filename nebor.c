//  This function generates the 2-dimensional matrix "nb", represented as "nbr" in the program, of dimensions (dim^2 X 6)
//  and is passed back to main for calculation purposes.
//  It stores neighbourhood information.
//  Created by Himakar Sreerangam on 02/10/19.
//  Copyright © 2019 Himakar Sreerangam. All rights reserved.
//



int **nebor(int dim){
    int L = dim;
    int a, b, x;
    int **nbr = (int**)malloc(sizeof(int*)*(L*L)); 

    for(a=0; a<L; a++){
        for(b=0; b<L; b++){
            x = a*L + b;
            
            nbr[x] = (int*)malloc(sizeof(int)*6); 
            
               
            if(a%2==0){
                nbr[x][0] = ((a+1)%L)*L + (b+1)%L; // 0th edge
                nbr[x][1] = a*L + (b+1)%L; // 1st edge
                nbr[x][2] = ((a-1+L)%L)*L +(b+1)%L; // 2nd edge
                nbr[x][3] = ((a-1+L)%L)*L +b; // 3rd edge
                nbr[x][4] = a*L + (b-1+L)%L; // 4th edge
                nbr[x][5] = ((a+1)%L)*L + b; //5th edge
            }
            else{
                nbr[x][0] = ((a+1)%L)*L + b; // 0th edge
                nbr[x][1] = a*L + (b+1)%L; // 1st edge
                nbr[x][2] = ((a-1+L)%L)*L +b; // 2nd edge
                nbr[x][3] = ((a-1+L)%L)*L +(b+L-1)%L; // 3rd edge
                nbr[x][4] = a*L + (b-1+L)%L; // 4th edge
                nbr[x][5] = ((a+1)%L)*L + (b+L-1)%L; //5th edge
            }
        }
    }
    return nbr;
}
