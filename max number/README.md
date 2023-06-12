# MAX NUMBER

You will develop a parallel program that outputs max number in a given array.
You will use Master-Slave paradigm where

Master:

- Reads size of array.
- Reads elements inside array.
- Distributes the work among slaves processes:
  • Sends size of the array.
  • Sends the assigned partition of the array.
- After each slave finishes its work, master process receives max number and its
  index from each process.
- Then master computes the max number from max numbers returned from
  each slave.
- Output the final max number and its index in the original array to user.

Slaves each one will:

- Receives size of the array.
- Receives the portion of the array.
- Calculates max number.
- Sends max number and its index back to master process.
  Note: Size of array may not be divisible by number of processes. So, you
  should handle this case.
