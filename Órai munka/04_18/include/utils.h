#define CL_TARGET_OPENCL_VERSION 220

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <CL/cl.h>

#ifndef UTILS_H
#define UTILS_H

bool isSorted(int arr1[], int size1);
int* generateValues(int* arr, int size);
float calculateMean(int arr[], int n);
float calculateSd(int arr[], int n);
bool hasCommonValue(int arr1[], int size1,int arr2[], int size2);
void exportToCsv(int SAMPLE_SIZE, cl_ulong kernel_execution_time);

#endif