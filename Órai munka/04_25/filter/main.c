#include <stdio.h>
#include <stdlib.h>

void average_filter(double *input, double *output, int length, int filter_size) {
    // Iterate over the input array
    for (int i = 0; i < length; i++) {
        double sum = 0;
        int count = 0;
        
        // Iterate over the filter window
        for (int j = i - filter_size/2; j <= i + filter_size/2; j++) {
            // Check for out-of-bounds indices
            if (j >= 0 && j < length) {
                sum += input[j];
                count++;
            }
        }
        
        // Compute the average and store it in the output array
        output[i] = sum / count;
    }
}

int main() {
    // Define the input signal and filter parameters
    double input[] = {0.1, 0.2, 0.3, 0.4, 0.5};
    int length = 5;
    int filter_size = 3;
    
    // Allocate memory for the output array
    double *output = (double*) malloc(length * sizeof(double));
    
    // Apply the average filter to the input signal
    average_filter(input, output, length, filter_size);
    
    // Print the input and output signals
    printf("Input signal: ");
    for (int i = 0; i < length; i++) {
        printf("%.2f ", input[i]);
    }
    printf("\n");
    
    printf("Output signal: ");
    for (int i = 0; i < length; i++) {
        printf("%.2f ", output[i]);
    }
    printf("\n");
    
    // Free the memory allocated for the output array
    free(output);
    
    return 0;
}
