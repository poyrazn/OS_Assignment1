//
//  assignment1.c
//  OS_Assignment1
//
//  Created by Nehir Poyraz on 8.03.2018.
//  Copyright © 2018 Nehir Poyraz. All rights reserved.
//

#include "assignment1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#define bool int
#define receiver 0
#define sender 1
#define done 0
#define executing 1

#define SIZE 2048

void *ptr;
const char *name = "OS";
int shm_fd;
int pipe_fd[2];
int pipe_fd2[2];
pid_t pid;

int main(int argc, const char * argv[]) {
    /* In this assignment, I have defined seperate functions for both parts. Using fork system call, 
    child process will execute part1 while parent waits for it to terminate. Then, parent continue to executing, 
    executes part2 */
    pid = fork();
    
    if (pid < 0 )
//         Child (executes part 1)
        perror("(main)\tFork failed");
    else if (pid == 0){
        fprintf(stderr,"\n\t***\tExecuting first part of the Assignment1\t***\t\n\n");
        part1();
    }
    else {
//         Parent waits for the child
        wait(NULL);
        fprintf(stderr, "\n\t***\tExecuting second part of the first Assignment1\t***\t\n\n");
        
        part2();
    }
    
    return 0;
}

int part1(){
    int number;
//     Establishing the shared memory object
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        printf("(part1)\tShared memory failed\n");
        exit(-1);
    }
    printf("(part1)\tShared memory segment has been opened.\n");
    ftruncate(shm_fd,SIZE);
    
    printf("\t\tEnter a positive integer number to find its Collatz sequence. Is it really going to reach 1? Let's see!\n");
    scanf("%d", &number);
    pid = fork();
    if (pid < 0) {
//         Error forking
        perror("(part1)\tFork failed");
    } else if (pid == 0) {
//         Child process
        printStat(1, 2, executing);
//         mapping for write
        ptr = mmap(0,SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED) {
            printf("(part1)\tChild Map failed\n");
            return -1;
        }
        Collatz(number);
        printStat(1, 2, done);
    }  else {
//         Parent process waits
        wait(NULL);
//         Child terminates, parent continues
        printStat(1, 1, executing);
//         mapping for read
        ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED) {
            printf("(part1)\tParent Map failed\n");
            exit(-1);
        }
        printf("(part1)\tCollatz sequence for the number %d is:\n", number);
        printf("\t\t%s\n",ptr);
    }
    return 0;
}
int part2() {
    size_t buf = 0;
    char *write_msg2;
    char read_msg[SIZE];
    
    if (pipe(pipe_fd) == -1) {
        fprintf(stderr,"Pipe1 failed");
        return 1;
    }
    if (pipe(pipe_fd2) == -1) {
        fprintf(stderr,"Pipe2 failed");
        return 1;
    }
    
    pid = fork();
    if (pid <0 )
        perror("(part2)\tFork failed");
    else if (pid == 0)
    {
//         Process 2
//         close unused ends of the pipes
        close(pipe_fd[1]);
        close(pipe_fd2[0]);

//         read the original message from the pipe1
        read(pipe_fd[0], read_msg, SIZE);
//         close the read end of the pipe1
        close(pipe_fd[0]);
        printStat(2, 2, receiver);
        printf("%s\n", read_msg);

//         Alter the message
        write_msg2 = toggleCase(read_msg);
//          write the altered message to the pipe2
        write(pipe_fd2[1], write_msg2, strlen(write_msg2)+1);
        printStat(2, 2, sender);
//         close the write end of the pipe2
        close(pipe_fd2[1]);
        
    }
    else
    {
        char *write_msg = NULL;
//         Process 1
//         close unused ends of the pipes
        close(pipe_fd[0]);
        close(pipe_fd2[1]);
        printf("(part2)\tProcess 1 is retrieving the input message...\n");
        printf("Type anything. The letters of the message you have typed will be toggled and send through the pipe:\n");
        getline(&write_msg, &buf, stdin);
//         write the original message to the pipe1
        write(pipe_fd[1], write_msg, strlen(write_msg)+1);
//         close the write end of the pipe1
        close(pipe_fd[1]);
        printStat(2, 1, sender);
        wait(NULL);
//         read the altered message from the pipe2
        read(pipe_fd2[0], read_msg, SIZE);
        printStat(2, 1, receiver);
        printf("%s\n", read_msg);
//         close the read end of the pipe2
        close(pipe_fd2[0]);
    }
    return 0;
}

void printStat(int partnum, int processnum, int mode) {
//      partnum: 1 for part1 & 2 for part2,
//      processnum: 1 for Process1, (Parent), 2 for Process2 (Child)
//      role: receiver (0) to read from pipe, sender (1) to write to the pipe
//      role: executing: 1 done: 0
    if (partnum == 1) {
        if (mode == executing) {
            if (processnum == 1)
                printf("\n(part%d)\t---\tParent process is executing...\n", partnum);
            else
                printf("\n(part%d)\t---\tChild process is executing...\n", partnum);
        }
        else
            printf("(part%d)\tChild is done.\n(part%d)\tThe sequence can be read from the shared memory.\n", partnum, partnum);
    }
    else {
        if (mode == receiver)
            printf("(part%d)\tProcess %d has received the message from the pipe: ", partnum, processnum);
        else if (mode == sender)
            printf("(part%d)\tProcess %d has written the message to the pipe\n", partnum, processnum);
    }
    
}



void Collatz(int n) {
//     this function both computes the elements of the sequence and pushes to the shared memory recursively
//      local buffer holds the integer value as char array
    char buffer [10];
    sprintf(buffer, "%d", n);
//     buffer to shm
    sprintf(ptr, "%s", buffer);
//     increase: buffer len
    ptr += strlen(buffer);
    if (n == 1) {
        return;
    }
    else {
        sprintf(ptr, " ");
        ptr++;
        if (n%2 == 0)
            n = n/2;
        else
            n = (3*n)+1;
        Collatz(n);
    }
}
char* toggleCase(char *msg){
//     allocating memory to hold the msg
    char *str = malloc(strlen(msg) +1);
    int i = 0;
    while(msg[i] != '\0') {
        if (msg[i] >= 'A' && msg[i] <= 'Z')
            str[i] = msg[i] + 32;
        else if (msg[i] >= 'a' && msg[i] <= 'z')
            str[i] = msg[i] - 32;
        else
            str[i] = msg[i];
        i++;
    }
    str[i] = msg[i];
    return str;
}
