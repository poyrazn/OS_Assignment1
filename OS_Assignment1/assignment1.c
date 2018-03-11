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
#define SIZE 2048
#define number 35
void *ptr;
const char *name = "OS";
int shm_fd;
int pipe_fd[2];
int pipe_fd2[2];
pid_t pid;

int main(int argc, const char * argv[]) {
    // insert code here...
    
    pid = fork();
    
    if (pid < 0 )
        perror("(main)\tFork failed");
    else if (pid == 0){
        printf("\n\t***\tExecuting first part of the Assignment1\t***\t\n\n");
        
        part1();
    }
    else {
        wait(NULL);
        printf("\n\n");
        printf("\t***\tExecuting second part of the first Assignment1\t***\t\n\n");
        part2();
    }
    
    return 0;
}

int part1(){
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        printf("(part1)\tShared memory failed\n");
        exit(-1);
    }
    printf("(part1)\tShared memory segment has been opened.\n");
    ftruncate(shm_fd,SIZE);
    pid = fork();
    if (pid < 0) {
        // Error forking
        perror("(part1)\tFork failed");
    } else if (pid == 0) {
        // Child process
        printf("\n(part1)\t---\tChild process is executing...\n");
        ptr = mmap(0,SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED) {
            printf("(part1)\tChild Map failed\n");
            return -1;
        }
        Collatz(number);
        printf("(part1)\tChild is done.\n(part1)\tThe sequence can be read from the shared memory.\n");
    }  else {
        // Parent process
        wait(NULL);
        printf("\n(part1)\t---\tParent process is executing...\n");
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
    char write_msg[SIZE] = "TRYInG SOMeThiNG DIFFErENT";
    char *write_msg2;
    char read_msg[SIZE];
    
    if (pipe(pipe_fd) == -1) {
        fprintf(stderr,"Pipe failed");
        return 1;
    }
    if (pipe(pipe_fd2) == -1) {
        fprintf(stderr,"Pipe failed");
        return 1;
    }
    pid = fork();
    if (pid <0 )
        perror("(part2)\tFork failed");
    else if (pid == 0)
    {
        
        close(pipe_fd[1]);
        close(pipe_fd2[0]);
        /*read the message from the pipe*/
        read(pipe_fd[0], read_msg, SIZE);
        close(pipe_fd[0]);
        printf("(part2)\tProcess 2 has received the message from the pipe:\n");
        printf("\noriginal message:\t%s\n\n", read_msg);
        /*Alter the message*/
        write_msg2 = toggleCase(read_msg);
        /* write to the pipe */
        write(pipe_fd2[1], write_msg2, strlen(write_msg2)+1);
        printf("(part2)\tProcess 2 has written the message to the pipe\n");
        /* close the write end of the pipe */
        close(pipe_fd2[1]);
        
    }
    else
    {
        // Parent
        close(pipe_fd[0]);
        close(pipe_fd2[1]);
        /* write the original message to the pipe */
        write(pipe_fd[1], write_msg, strlen(write_msg)+1);
        close(pipe_fd[1]);
        printf("(part2)\tProcess 1 has written the message to the pipe\n");
        wait(NULL);
        read(pipe_fd2[0], read_msg, SIZE);
        printf("(part2)\tProcess 1 has received the message from the pipe:\n");
        printf("\naltered message:\t%s\n\n", read_msg);
        close(pipe_fd2[0]);
    }
    return 0;
}

void Collatz(int n) {
    char buffer [10];
    sprintf(buffer, "%d", n);
    sprintf(ptr, "%s", buffer);
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
