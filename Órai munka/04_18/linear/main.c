#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const int SAMPLE_SIZE = 262144;

int* myAlgo(int buffer[], int n);
void exportToCsv(int SAMPLE_SIZE, double kernel_execution_time);

int main(void)
{
    clock_t start_time, end_time;
    int* buffer = (int*)malloc(SAMPLE_SIZE * sizeof(int));
    srand(0);
    
    for (int i = 0; i < SAMPLE_SIZE; ++i) {
        buffer[i] = rand() % 1000;
    }

    start_time = clock();

    int* result = myAlgo(buffer, SAMPLE_SIZE);

    end_time = clock();

    for (int i = 0; i < SAMPLE_SIZE; ++i) {
        printf("[%d] = %d, ", i, result[i]);
    }

    double linear_execution_time = ((double) (end_time - start_time)) / (CLOCKS_PER_SEC / 1000);
    printf("\nLinear execution time: %0.3f ms\n", linear_execution_time);
    exportToCsv(SAMPLE_SIZE, linear_execution_time);

    printf("Ready!\n");
}

int* myAlgo(int buffer[], int n) {
    int* result = (int*)malloc(n * sizeof(int));  // allocate memory for result array

    for (int i = 0; i < n; i++) {
        int sum = 0;

        // calculate sum of 3 neighboring elements
        for (int j = i - 1; j <= i + 1; j++) {
            if (j >= 0 && j < n) {
                sum += buffer[j];
            }
        }

        // calculate average of 3 neighboring elements
        result[i] = sum / 3;
    }

    return result;
}

void exportToCsv(int SAMPLE_SIZE, double kernel_execution_time){
    // Open the output file
    time_t t = time(NULL);
    struct tm *current_time = localtime(&t);

    char datetime_str[25];
    strftime(datetime_str, sizeof(datetime_str), "%Y_%m_%d_%H_%M_%S", current_time);
    printf("Current datetime: %s\n", datetime_str);
    char str2[] = "_output.csv";
    strcat(datetime_str, str2);
    FILE* output_file = fopen(datetime_str, "w");

    // Write the header row
    fprintf(output_file, "SAMPLE_SIZE,kernel_execution_time\n");

    // Write the data row
    fprintf(output_file, "%d,%0.3f\n", SAMPLE_SIZE, kernel_execution_time);

    // Close the output file
    fclose(output_file);
}