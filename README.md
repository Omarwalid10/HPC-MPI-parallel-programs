# HPC-MPI-parallel-programs
High Performance Computing problems with MPI 
### How to run the MPI code in Ubuntu
- sudo apt-get install mpich
- mpicc YourSouceCode.c -o output.c
- mpiexec -n 3 ./output.c (n is the number of cores you want to use them)
