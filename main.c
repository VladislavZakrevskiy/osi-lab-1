#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define MAX_NUMBERS 100
#define BUFFER_SIZE 1024

int main() {
    int pipe_fd[2];
    pid_t child_pid;
    char input_line[BUFFER_SIZE];
    float numbers[MAX_NUMBERS];
    int count;
    
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
        
        FILE *output_file = fopen("result.txt", "w");
        if (output_file == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        
        while (1) {
            int numbers_count;
            if (read(pipe_fd[0], &numbers_count, sizeof(int)) <= 0) {
                break;
            }
            
            if (numbers_count == 0) {
                break;
            }
            
            float numbers[numbers_count];
            if (read(pipe_fd[0], numbers, sizeof(float) * numbers_count) <= 0) {
                perror("read");
                break;
            }
            
            float sum = 0.0;
            for (int i = 0; i < numbers_count; i++) {
                sum += numbers[i];
            }
            
            fprintf(output_file, "Numbers: ");
            for (int i = 0; i < numbers_count; i++) {
                fprintf(output_file, "%.2f ", numbers[i]);
            }
            fprintf(output_file, "| Sum: %.2f\n", sum);
            fflush(output_file);
            
            printf("Child process calculated sum: %.2f\n", sum);
        }
        
        fclose(output_file);
        close(pipe_fd[0]);
        exit(EXIT_SUCCESS);
    } else {
        close(pipe_fd[0]);
        
        printf("Enter numbers separated by spaces (empty line to exit):\n");
        
        while (fgets(input_line, sizeof(input_line), stdin)) {
            if (input_line[0] == '\n') {
                break;
            }
            
            count = 0;
            char *token = strtok(input_line, " \t\n");
            
            while (token != NULL && count < MAX_NUMBERS) {
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
                if (write(pipe_fd[1], &count, sizeof(int)) == -1) {
                    perror("write count");
                    break;
                }
                
                if (write(pipe_fd[1], numbers, sizeof(float) * count) == -1) {
                    perror("write numbers");
                    break;
                }
                
                printf("Sent %d numbers to child process\n", count);
            }
        }
        
        count = 0;
        write(pipe_fd[1], &count, sizeof(int));
        
        close(pipe_fd[1]);
        
        int status;
        if (wait(&status) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
        
        if (WIFEXITED(status)) {
            printf("Child process finished with exit code: %d\n", WEXITSTATUS(status));
        }
        
        printf("Results saved to result.txt\n");
    }
    
    return 0;
}
