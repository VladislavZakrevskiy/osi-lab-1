#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

void child_process(int read_fd) {
    FILE *output_file = fopen("result.txt", "w");
    if (output_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        int numbers_count;
        if (read(read_fd, &numbers_count, sizeof(int)) <= 0) {
            break;
        }
        
        if (numbers_count == 0) {
            break;
        }
        
        float numbers[numbers_count];
        if (read(read_fd, numbers, sizeof(float) * numbers_count) <= 0) {
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
    close(read_fd);
    exit(EXIT_SUCCESS);
}
