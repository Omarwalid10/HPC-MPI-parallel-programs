#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, processes, i, max, maxIndex, start, end, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processes);
    if (rank == 0) {
        // master process reads the size of the array and the elements
        printf("Enter the size of the array : ");
        scanf("%d", &size);
        int* array = (int*)malloc(size * sizeof(int));
        printf("Enter the elements :\n");
        for (i = 0; i < size; i++) {
            scanf("%d", &array[i]);
        }
        printf("From( MASTER )the number of slaves is %d .\n",processes-1);
        // master process distributes the work among the slave processes
        for (i = 1; i < processes; i++) {
            start = (i - 1) * (size / (processes - 1));
            if (i == processes - 1) {
                end = size;
            } else {
                end = i * (size / (processes - 1));
            }
            int prosesSize= end - start;
            // send the partition size first to allocate new array with this size before send the array
            MPI_Send(&prosesSize, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&array[start], prosesSize, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        // master process receives the max number and its index from each slave
        max = array[0];
        maxIndex = 0;
        for (i = 1; i < processes; i++) {
            int slave_max, slave_max_index;
            MPI_Recv(&slave_max, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&slave_max_index, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
             printf("Hello form slave %d the max num is %d at index %d in my array.\n",i,slave_max,slave_max_index);
            if (slave_max > max) {
                max = slave_max;
                maxIndex = slave_max_index;
            }
        }
        printf("From( MASTER )the maximum number in the array is( %d ) at index %d.\n", max, maxIndex);
        free(array);

    } else {
        //Slaves
        // slave processes receive the size of the array 
        MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int* partition = (int*)malloc(size * sizeof(int));
        MPI_Recv(partition, size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //   calculate the max number and its index  
        max = partition[0];
        maxIndex = 0;
        for (i = 1; i < size; i++) {
            if (partition[i] > max) {
                max = partition[i];
                maxIndex = i;
            }
        }
        // slave processes send the max to the master
        MPI_Send(&max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&maxIndex, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        free(partition);
    }

    MPI_Finalize();
    return 0;
}