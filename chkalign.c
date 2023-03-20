// This function checks different types of alignments 
// and updates the 'align' matrix with that information. 




void chkalign(double align[], int **spin, int **nbr, int totalcell, int *upcell, int  **alignMatrix){
    
    
    double c_IA = 0; //INTERNALLY aligned 
    double c_EA = 0; //EXTERNALLY aligned 
    double c_AA = 0; //COMPLETELY aligned 
    double c_AU = 0; // Aligned UP


    
    for (int i = totalcell; i--; ){
        
        //Set all to zero
        alignMatrix[i][0] = 0;
        alignMatrix[i][1] = 0;
        alignMatrix[i][2] = 0;
        alignMatrix[i][3] = 0;
        upcell[i]=0;
        
        // Checking internal alignment
        if(spin[i][0]*spin[i][1]+spin[i][1]*spin[i][2]+spin[i][2]*spin[i][3]+spin[i][3]*spin[i][4]+spin[i][4]*spin[i][5]+spin[i][5]*spin[i][0] == 2){
                alignMatrix[i][0] = 1;
        }
            
        // Checking external alignment
        if(spin[i][0]*spin[nbr[i][0]][3] + spin[i][1]*spin[nbr[i][1]][4] + spin[i][2]*spin[nbr[i][2]][5] + spin[i][3]*spin[nbr[i][3]][0] + spin[i][4]*spin[nbr[i][4]][1] + spin[i][5]*spin[nbr[i][5]][2] == -4){
            alignMatrix[i][1] = 1;
        }

        
        // Updating each element of the array ALIGN :

        c_IA = c_IA + alignMatrix[i][0];
     
        c_EA = c_EA + alignMatrix[i][1];

        
        // Checking complete alignment
        alignMatrix[i][2] = (alignMatrix[i][0] + alignMatrix[i][1])/2.0;
        if(alignMatrix[i][2] == 1){

            c_AA = c_AA + alignMatrix[i][2];

                //Checking directionality
               if(spin[i][0]+ 2*spin[i][1] == 1){
                    alignMatrix[i][3] = 1; //Aligned upward
                    upcell[i] = 1;
                 
                }

                c_AU = c_AU + alignMatrix[i][3];
        
        }
        
    }
    
  align[0] = c_IA/totalcell;
  align[1] = c_EA/totalcell;
  align[2] = c_AA/totalcell;
  align[3] = c_AU/totalcell;

}
