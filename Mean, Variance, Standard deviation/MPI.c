#include <stdio.h>

#include <math.h>

#include <mpi.h>

#include <time.h>

#define MAXSIZE 100

double sqrtt(int number)

{

    int start = 0, end = number;

    int mid;

    float ans;



    while (start <= end) {

        mid = (start + end) / 2;



        if (mid * mid == number) {

            ans = mid;

            break;

        }



        if (mid * mid < number) {

            ans = start;

            start = mid + 1;

        }

        else {

            end = mid - 1;

        }

    }



    float increment = 0.1;

    for (int i = 0; i < 5; i++) {

        while (ans * ans <= number) {

            ans += increment;

        }



        ans = ans - increment;

        increment = increment / 10;

    }



    return ans;

}



int main()

{







    double parallel=0;



    clock_t startTime,endTime;



    int rank, size, i, n,chunk,start ,end;

    float x[MAXSIZE];

    float average, variance, std_deviation, sum = 0, sum1 = 0;

    float local_sum = 0, local_sum1 = 0;

    float local_average, local_variance, local_std_deviation;



    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);



    if (rank == 0)

    {

        startTime=clock();



        printf("Enter the value of N \n");

        scanf("%d", &n);

        printf("Enter %d real numbers \n", n);

        for (i = 0; i < n; i++)

        {

            scanf("%f", &x[i]);

        }

        chunk=n/size;









    }





    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(x, n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    MPI_Bcast(&chunk, 1,MPI_INT, 0, MPI_COMM_WORLD);

    start =rank * chunk;

    if(rank!=size-1){

        end =start +chunk;

    }

    else{

        end =n;

    }

    /* Compute the local sum of elements */

    for (i = start ; i < end; i++)

    {

        local_sum += x[i];

        // printf("local sum= %.2f %d\n", local_sum,rank);

    }



    /* Reduce the local sums to obtain the global sum */

    MPI_Reduce(&local_sum, &sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);



    /* Compute the global average */

    if(rank==0){

        average = sum / n;

    }

    /* Compute the local sum of squared differences from the average */

    MPI_Bcast(&average, 1,MPI_FLOAT, 0, MPI_COMM_WORLD);

    for (i = start; i < end; i++)

    {

        local_sum1 += (((x[i] - average)*(x[i] - average)));

        // printf("x[i]= %.2f %f %f ,\n", x[i],(x[i]-average),average);

        // printf("local sum1= %.2f,%d\n", local_sum1,rank);

    }



    /* Reduce the local sums of squared differences to obtain the global sum */

    MPI_Reduce(&local_sum1, &sum1, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);



    /* Compute the global variance and standard deviation */

    if (rank == 0)

    {

        variance = sum1 / n;

        std_deviation = sqrtt(variance);

    }



    /* Broadcast the results to all processes */

    MPI_Bcast(&average, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    MPI_Bcast(&variance, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    MPI_Bcast(&std_deviation, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);



    /* Print the results */

    if (rank == 0)

    {

        printf("Mean of all elements = %.2f\n", average);

        printf("Variance of all elements = %.2f\n", variance);

        printf("Standard deviation = %.2f\n", std_deviation);





        endTime=clock();

        parallel=(endTime-startTime);



        printf("--------------------------------------------------------------------\n");

        printf("the parallelTime = %.2f\n", parallel);

    }



    MPI_Finalize();

    return 0;

}

