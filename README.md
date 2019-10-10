# Interprocess Communication (IPC)

A process is a program in execution, which has its own address space that spans the memory locations accessible by the process. A process has at least one thread, it may also have multiple threads. Some resources are shared among threads within same process, such as address space. 

Inter process communication (IPC) is a mechanism which allows the communication and synchronization between processes.
Communication can occur in two ways: **shared memory** and **message passing**. In this project, both techniques for managing IPC are implemented. 

In Part 1, the communication link is established using a shared memory object, whereas in Part 2, processes communicate through pipes instead of shared memory segment. 

### Part 1 (Collatz Conjecture)

*The Collatz conjecture is a conjecture in mathematics that concerns a sequence defined as follows: start with any positive integer n. Then each term is obtained from the previous term as follows: if the previous term is even, the next term is one half the previous term. If the previous term is odd, the next term is 3 times the previous term plus 1. The conjecture is that no matter what value of n, the sequence will always reach 1.*


The goal is to manage IPC using shared memory, to achieve this two processes are created. One process is responsible from receiving an integer letter as input from the user, storing it on shared memory then wait until the other process is terminated and pull the sequence from shared memory. The other process reads the number from shared memory, finds its Collatz sequence and pushes it back to the memory.

### Part 2 (Inverted Letter Cases)

In this part, the goal is to manage IPC via pipes. Similarly, one process is given a string and writes it into a pipe and wait for reading, whereas the other process reads the message from the pipe, toggles its letters (e.g. a-->A or C-->c)and sends it back through the pipe. 

