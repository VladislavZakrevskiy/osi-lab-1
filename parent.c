#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

void parent_process(int write_fd) {
    char *input_line = NULL;
    size_t line_size = 0;
    ssize_t line_length;
    float *numbers = NULL;
    int count;
    int capacity = 10;
    
    numbers = malloc(capacity * sizeof(float));
    if (numbers == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    printf("Enter numbers separated by spaces (empty line to exit):\n");
    
    while ((line_length = getline(&input_line, &line_size, stdin)) != -1) {
        if (line_length == 1 && input_line[0] == '\n') {
            break;
        }
        
        count = 0;
        char *token = strtok(input_line, " \t\n");
        
        while (token != NULL) {
            if (count >= capacity) {
                capacity *= 2;
                numbers = realloc(numbers, capacity * sizeof(float));
                if (numbers == NULL) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }
            }
            char *endptr;
            float num = strtof(token, &endptr);
            
            if (*endptr != '\0') {
                printf("Invalid number: %s\n", token);
                token = strtok(NULL, " \t\n");
                continue;
            }
            
            numbers[count++] = num;
            token = strtok(NULL, " \t\n");
        }
        
        if (count > 0) {
            if (write(write_fd, &count, sizeof(int)) == -1) {
                perror("write count");
                break;
            }
            
            if (write(write_fd, numbers, sizeof(float) * count) == -1) {
                perror("write numbers");
                break;
            }
            
            printf("Sent %d numbers to child process\n", count);
        }
    }
    
    count = 0;
    write(write_fd, &count, sizeof(int));
    
    close(write_fd);
    
    int status;
    if (wait(&status) == -1) {
        perror("wait");
        exit(EXIT_FAILURE);
    }
    
    if (WIFEXITED(status)) {
        printf("Child process finished with exit code: %d\n", WEXITSTATUS(status));
    }
    
    printf("Results saved to result.txt\n");
    
    free(input_line);
    free(numbers);
}
