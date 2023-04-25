#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

void fft(double complex *x, int n) {
    if (n <= 1) {
        return;
    }
    
    // divide
    double complex *even = (double complex*) malloc(n/2 * sizeof(double complex));
    double complex *odd = (double complex*) malloc(n/2 * sizeof(double complex));
    for (int i = 0; i < n/2; i++) {
        even[i] = x[2*i];
        odd[i] = x[2*i+1];
    }
    
    // conquer
    fft(even, n/2);
    fft(odd, n/2);
    
    // combine
    for (int k = 0; k < n/2; k++) {
        double complex t = cexp(-2.0 * M_PI * I * k / n) * odd[k];
        x[k] = even[k] + t;
        x[k+n/2] = even[k] - t;
    }
    
    free(even);
    free(odd);
}

int main() {
    int n = 8;
    double complex x[] = {1, 2, 3, 4, 4, 3, 2, 1};
    
    fft(x, n);
    
    for (int i = 0; i < n; i++) {
        printf("x[%d] = %f + %f i\n", i, creal(x[i]), cimag(x[i]));
    }
    
    return 0;
}