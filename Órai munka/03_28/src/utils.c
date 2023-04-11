#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int generateValues(int arr[], int size){
    //int arr[10000];
    int seed;
    printf("set seed: ");
    scanf("%d", &seed);
    printf("\n");
    srand(seed);
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 1000;
        printf("%d ",arr[i]);
    }
    
    return arr;
}

bool isSorted(int arr1[], int size1) {   
    bool res = true; 
    for(int i = 1; i < size1; i++) {
        if(!(arr1[i-1] <= arr1[i])){
            res = false;
    }
    }
    if(res){
        printf("\nsorted\n");
    }
    else{
        printf("\nnot sorted\n");
    }
    return res;
}


float calculateMean(int arr[], int n) {
    float sum = 0;

    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    return sum / n;
}

float calculateSd(int arr[], int n) {
    float mean = calculateMean(arr, n);
    float sum = 0;

    for (int i = 0; i < n; i++) {
        sum += pow(arr[i] - mean, 2);
    }

    float sd = sqrt(sum / n);
    return sd;
}

bool hasCommonValue(int arr1[], int size1,int arr2[], int size2){
    for(int i = 0; i < size1; i++){
        for(int j = 0; j < size2; j++){
            if(arr1[i]==arr2[j]){
                return true;
            }
        }
    }
    return false;
}