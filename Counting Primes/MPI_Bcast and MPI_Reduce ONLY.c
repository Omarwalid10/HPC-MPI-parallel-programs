#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

int prime(int num) {

    if (num <= 1) {
        return 0;
    }
    for (int i = 2; i*i <=  num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    int rank, numOfProcesses;
    int x, y, r, glopalCount = 0, rangeCount = 0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcesses);
    clock_t startTime;
    if (rank == 0) {
        startTime=clock();
        printf("Enter x lower : ");
        scanf("%d", &x);
        printf("Enter y upper : ");
        scanf("%d", &y);
    }
    //sent x y form 0 to all 
    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&y, 1, MPI_INT, 0, MPI_COMM_WORLD);

    r = (y - x) / numOfProcesses;
    //to include the lower process 0 from start (that mean master include 1 number more) 
    int lower ;
    if(rank == 0){
        lower  = x + rank * r ;
    }
    else{ 
        lower = x + rank * r + 1;
    }
    int upper;
    //to handle if lenth of range not divisible by the numper of process put the reminders in last
    if(rank == numOfProcesses-1){
        upper = y;
    }
    else {
        upper= x + (rank + 1) * r;
    }
    // printf("in process %d from %d to %d \n",rank,lower,upper);
    for (int i = lower; i <= upper; i++) {
        if (prime(i)) {
            rangeCount++;
           // printf("prime %d in process %d \n",i,rank);
        }
    }
    printf("from %d to %d the sum =( %d ) in process ( %d )\n",lower,upper,rangeCount,rank);
    //take partial count and add it in glopal count to print it in process 0
    MPI_Reduce(&rangeCount, &glopalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("---------------------------------------------------------\n");
        printf("Total count of prime numbers between %d and %d: %d\n", x, y, glopalCount);
        double endTime=(double)(clock()-startTime)/CLOCKS_PER_SEC;
        printf("----------->  the time = %f \n", endTime);
        printf("---------------------------------------------------------\n");
    }

    MPI_Finalize();
    return 0;
}
