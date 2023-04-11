#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#ifndef UTILS_H
#define UTILS_H

bool isSorted(int arr1[], int size1);
int generateValues(int arr[], int size);
float calculateMean(int arr[], int n);
float calculateSd(int arr[], int n);
bool hasCommonValue(int arr1[], int size1,int arr2[], int size2);

#endif