#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

void child_process(int read_fd);
void parent_process(int write_fd);

int main() {
    int pipe_fd[2];
    pid_t child_pid;
    
    if (pipe(pipe_fd) == -1) { 
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (child_pid == 0) {
        close(pipe_fd[1]);
        child_process(pipe_fd[0]);
    } else {
        close(pipe_fd[0]);
        parent_process(pipe_fd[1]);
    }
    
    return 0;
}