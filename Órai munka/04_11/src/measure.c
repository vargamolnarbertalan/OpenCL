#include "measure.h"

#include <stdio.h>
#include <time.h>

double calculateRuntime(clock_t start_time, clock_t end_time){
    double total_time;

    total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Calculate the total time in seconds

    printf("\nTotal runtime: %f seconds\n", total_time);

    return total_time;
}
