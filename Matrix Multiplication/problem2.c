 //make sure to import these libraries to avoid errors.
#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_2d(int **ary,int row,int col)
{
    for (int i = 0; i <row; ++i) {
        for (int j = 0; j < col; ++j) {
            printf("%d ",ary[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv) {

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    //make variables for all processes.
    int rows,col; int **array;
    int r1,c1; int **m1;
    int r2,c2; int **m2;
    int r3,c3; int **m3;

    // take variable input at master process.
    if(rank==0)
    {
        int ch;
        printf("Enter 1 for file and 2 for console: \n");
        scanf(" %d",&ch);
        if(ch==2){ 
        printf("Enter dimentions of the first matrix: ");
        fflush(stdout); //flush after printing so it appeares before input.
        scanf("%d %d",&r1,&c1);
        //building the 2d array.
        m1 = (int**)malloc(r1 * sizeof(int));
        for (int i = 0; i < r1; i++)
        m1[i] = (int*)malloc(c1* sizeof(int));
        //taking array elements as input.
        printf("Enter elements of the the first matrix: \n");
        fflush(stdout);
        for (size_t i = 0; i < r1; i++)
        {
            for (size_t j = 0; j < c1; j++)
            {
                scanf("%d",&m1[i][j]);
            }
            
        }
        printf("\n");
        printf("Enter dimentions of the second matrix: ");
        fflush(stdout); //flush after printing so it appeares before input.
        scanf("%d %d",&r2,&c2);
        //building the 2d array.
        m2 = (int**)malloc(r2 * sizeof(int));
        for (int i = 0; i < r2; i++)
        m2[i] = (int*)malloc(c2* sizeof(int));
        //taking array elements as input.
        printf("Enter elements of the the second matrix: \n");
        fflush(stdout);
        for (size_t i = 0; i < r2; i++)
        {
            for (size_t j = 0; j < c2; j++)
            {
                scanf("%d",&m2[i][j]);
            }  
        }
        printf("\n");
        if(c1==r2){
            r3=r1;
            c3=c2;
        }
        else{
            printf("you should enter valid dimentions \n");
            return 0;   
        }
        }
        else{
            freopen ("textf.txt", "r", stdin);
            freopen ("textf.txt", "w", stdout);
        }
        m3 = (int**)malloc(r3 * sizeof(int));
        for (int i = 0; i < r3; i++)
        m3[i] = (int*)malloc(c3* sizeof(int));
        // print_2d(m1,r1,c1);
        // print_2d(m2,r2,c2);
        // print_2d(m3,r3,c3);
       // sending rows and columns to all processes.
       int processSize=r1/(size-1),i,j,c=0;//num of row for each process
        for (  i = 1; i < size; i++)
        {
        //    if(i<size-1){
        //     c+=processSize;
            MPI_Send( &processSize , 1 , MPI_INT, i ,0, MPI_COMM_WORLD);
            MPI_Send( &c1 , 1 , MPI_INT, i ,0, MPI_COMM_WORLD);
            for (j = processSize*(i-1); j <processSize*(i) ; j++)//loops through the rows of the 2d array and sends each row.
            {
                MPI_Send(m1[j], c1, MPI_INT, i, 0, MPI_COMM_WORLD); //send (1d array of current row, which has a size = number of columns).
            }
            // }
            // else{
            // int sizze=r1-c;
            // MPI_Send( &sizze, 1 , MPI_INT, i ,0, MPI_COMM_WORLD);
            // MPI_Send( &c1 , 1 , MPI_INT, i ,0, MPI_COMM_WORLD);
            //      for (j = processSize*(i-1); j <size ; j++)//loops through the rows of the 2d array and sends each row.
            // {
            //     MPI_Send(m1[j], c1, MPI_INT, i, 0, MPI_COMM_WORLD); //send (1d array of current row, which has a size = number of columns).
            // }
            // }
            MPI_Send( &r2, 1 , MPI_INT, i ,0, MPI_COMM_WORLD);
            MPI_Send( &c2 , 1 , MPI_INT, i ,0, MPI_COMM_WORLD);
             for (j = 0; j <r2 ; j++)//loops through the rows of the 2d array and sends each row.
            {
                MPI_Send(m2[j], c2, MPI_INT, i, 0, MPI_COMM_WORLD); //send (1d array of current row, which has a size = number of columns).
            }
        }
        //resive m3 form the processes
        int Res;

        for ( int i = 1; i < size; i++)
        {
           MPI_Recv(&Res, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
           for (j = Res*(i-1); j < Res*(i); j++)
           {
              MPI_Recv(m3[j], c3, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
           }
        }
        printf("from ((( master ))) process the result is :\n");
        print_2d(m3,r3,c3);

        //because you can't send a 2d array in one go
        //you will send each individual row in the array.
        
    }
    else
    {
        // //now each other process receives the values of rows and columns.
        //recive first
        MPI_Recv(&r1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&c1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("r= %d c= %d for process %d \n",r1,c1,rank);
        m1 = (int**)malloc(r1 * sizeof(int));
        for (int i = 0; i < r1; i++)
        m1[i] = (int*)malloc(c1* sizeof(int));

        for (size_t j = 0; j <r1; j++)
        {
            MPI_Recv(m1[j], c1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printf("printing the 2d array at process %d:- \n",rank);
        print_2d(m1,r1,c1);
        //resive second
        MPI_Recv(&r2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&c2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        m2 = (int**)malloc(r2 * sizeof(int));
        for (int i = 0; i < r2; i++)
        m2[i] = (int*)malloc(c2* sizeof(int));
         for (size_t j = 0; j <r2; j++)
        {
            MPI_Recv(m2[j], c2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printf("printing the 2d array that i will multiply it :- \n");
        print_2d(m2,r2,c2);
        //make m3 to take reselt
        r3=r1;
        c3=c2;
        m3 = (int**)malloc(r3 * sizeof(int));
        for (int i = 0; i < r3; i++)
        m3[i] = (int*)malloc(c3* sizeof(int));
        int value=0;
         for (size_t i = 0; i < r3; i++)
        {
            for (size_t j = 0; j < c3; j++)
            {
                value=0;
                for (size_t k = 0; k < c3; k++)
                {
                    value+=m1[i][k]*m2[k][j];
                }
                
                m3[i][j]=value;
            }
            
        }
        printf("the result of process %d \n",rank);
        print_2d(m3,r3,c3);
        //send this matrix to master
        MPI_Send( &r3, 1 , MPI_INT, 0 ,0, MPI_COMM_WORLD);
        for (size_t j = 0; j <r3 ; j++)//loops through the rows of the 2d array and sends each row.
        {
                MPI_Send(m3[j], c3, MPI_INT, 0, 0, MPI_COMM_WORLD); //send (1d array of current row, which has a size = number of columns).
        }

    }
    MPI_Finalize();
    return 0;
}