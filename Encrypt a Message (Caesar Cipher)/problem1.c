#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mpi/mpi.h"

#define MAX_MSG_SIZE 1000000
 
int main(int argc, char *argv[]) {
    int key, messageSize, partLen, start, end;
    char input_msg[MAX_MSG_SIZE], part_msg[MAX_MSG_SIZE], output_msg[MAX_MSG_SIZE],filename[256];
    MPI_Status status;
    char *recv_buffer = NULL;
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter key : ");
        scanf("%d", &key);
        }
        FILE *f;
        strcpy(filename,"INPUT.txt");
        f = fopen(filename, "r");
        if (f== NULL) {
            printf("ERROR enter the correct file.\n");
            return 1;
        }
        else{
        fseek(f, 0, SEEK_END);
        messageSize = ftell(f);
        fseek(f, 0, SEEK_SET);
        fread(input_msg, sizeof(char), messageSize, f);
        fclose(f);

       
    }
    MPI_Bcast(&key, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // scatter message on all processes
    int rangeSize= MAX_MSG_SIZE / size;
    MPI_Scatter(input_msg, rangeSize, MPI_CHAR, part_msg, rangeSize, MPI_CHAR, 0, MPI_COMM_WORLD);

    // handle  the end of the message if it remaining
    if (rank == 0) {
        start = rangeSize * size;
        end = messageSize;
        partLen = end - start;
        if (partLen > 0) {
            strncpy(part_msg, &input_msg[start], partLen);
        }
    }
   
    for (int i = 0; i < strlen(part_msg); i++) {
        //to not include anthor characters 
        if (isalpha(part_msg[i])) {
            //to handle if upper or lower
            if (isupper(part_msg[i])) 
            {
                output_msg[i] = ((part_msg[i] - 'A' + key) % 26) + 'A';
            } else 
            {
                output_msg[i] = ((part_msg[i] - 'a' + key) % 26) + 'a';
            }
        } 
        else {
            output_msg[i] = part_msg[i];
        }
    }

    MPI_Gather(output_msg, rangeSize, MPI_CHAR, input_msg, rangeSize, MPI_CHAR, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        FILE *f;
        f = fopen("OUTPUT.txt", "w");
        fwrite(output_msg, sizeof(char), messageSize, f);
        fclose(f);
    }

    MPI_Finalize();
    return 0;
}