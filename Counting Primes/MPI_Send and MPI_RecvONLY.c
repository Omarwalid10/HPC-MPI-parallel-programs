#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>
int is_prime(int n) {
    if (n <= 1) {
        return 0;
    }
    for (int i = 2; i*i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char** argv) {
    int p, rank;
    int x, y;
    int r, a, b;
    int partial_count = 0, total_count = 0;
    clock_t startTime;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        startTime=clock();
        printf("Enter lower bound number x: ");
        scanf("%d", &x);
        printf("Enter upper bound number y: ");
        scanf("%d", &y);
        r = (y - x) / (p - 1);
        // Send x, y and r to all slave processes
        for (int i = 1; i < p; i++) {
            MPI_Send(&x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&y, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&r, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        a = x + (rank - 1) * r + 1;
        if (rank == p - 1) {
            b = y;
        } else {
            b = a + r - 1;
        }
    } else {
        // Receive x, y and r from the master process
        MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&y, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&r, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        a = x + (rank - 1) * r + 1;
        if (rank == p - 1) {
            b = y;
        } else {
            b = a + r - 1;
        }
    }
    for (int i = a; i <= b; i++) {
        if (is_prime(i)) {
            partial_count++;
        }
    }
    if (rank == 0) {
        // Receive partial counts from all slave processes
        for (int i = 1; i < p; i++) {
            MPI_Recv(&partial_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_count += partial_count;
        }
        printf("---------------------------------------------------------\n");
        printf("Total number of prime numbers between %d and %d is %d\n", x, y, total_count);
        double endTime=(double)(clock()-startTime)/CLOCKS_PER_SEC;
        printf("----------->  the time = %f \n", endTime);
        printf("---------------------------------------------------------\n");
    } else {
        // Send partial count to the master process
        MPI_Send(&partial_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
         printf("from %d to %d the sum =( %d ) in process ( %d )\n",a,b,partial_count,rank);
    }
    MPI_Finalize();
    return 0;
}
