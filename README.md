# Process Control and Shared Memory Simulation

This project simulates several aspects of process control and shared memory in a Unix-like environment using C programming. It consists of three problems, each focusing on different concepts.

## Problem 1: Forks

### Part 1
- In this part, the parent process creates a single child process.
- The child process continuously outputs the time of the day and its own PID every second.
- The parent process waits for 5 seconds and then kills the child process, printing "Child <PID> killed."
- After killing the child, the parent terminates.

### Part 2
- In this part, the parent process creates 4 child processes.
- Each child process continuously outputs the time of the day and its own PID every second.
- The parent process waits for 5 seconds and then kills all child processes, printing "Child <PID> killed" for each child.
- After killing the children, the parent terminates.

## Problem 2: Pipes

### Part 1
- The parent process creates 2 child processes.
- The parent reads an integer from the standard input and sends it to the children through pipes.
- The first child multiplies the input by its own PID, prints the result, and sends it back to the parent.
- The second child divides its PID by the input, prints the result, and sends it back to the parent.
- The parent sums both outputs and prints the final result.
- The parent kills all child processes and terminates.

### Part 2
- The same operations are performed as in Part 1, but each process has a sleep(5) parameter for tracking behavior.
- The program execution can be monitored using the `htop` tool, displaying the process tree hierarchy.
- The first child process can be killed during execution, and its termination can be observed in the program's output.

## Problem 3: Shared Memory - Chinese Whispers

### Driver Program
- This program simulates the Chinese whispers game by creating N child processes.
- It accepts three command line arguments: the executable name of the "consumer-producer program," the number of processes (N), and a string message.
- The driver program forks N children, and each child plays the game by executing the consumer-producer program with the necessary arguments.
- A minimum of 2 processes must be created; otherwise, the program terminates.
- The driver program creates a new child only after the current child terminates to prevent concurrent writes to shared memory.

### Consumer-Producer Program
- If the program is run by the first child of the driver program, it creates a shared memory segment and writes the original message into it.
- For all other children, the program reads the message in the shared segment, randomly chooses two characters in the message, swaps them, and writes the distorted message back into the shared segment.
- If it is run by the last child, it also unlinks the shared memory segment.
