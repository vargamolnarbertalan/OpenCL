#define CL_TARGET_OPENCL_VERSION 220

#include "kernel_loader.h"

#include "utils.h"

#include <stdio.h>

#include <stdlib.h>

#include <CL/cl.h>

int main() {
   int ARRAY_SIZE = -1;
   printf("set array size: ");
   scanf("%d", & ARRAY_SIZE);
   int input_array[ARRAY_SIZE];
   int input_array2[ARRAY_SIZE];
   int output_array[ARRAY_SIZE];
   int output_array2[ARRAY_SIZE];

   cl_platform_id platform;
   cl_device_id device;
   cl_context context;
   cl_command_queue queue;
   cl_program program;
   cl_kernel kernel;
   cl_int err;

   // Initialize OpenCL context
   err = clGetPlatformIDs(1, & platform, NULL);
   err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, & device, NULL);
   context = clCreateContext(NULL, 1, & device, NULL, NULL, & err);
   // queue = clCreateCommandQueue(context, device, 0, &err); --- depricated
   queue = clCreateCommandQueueWithProperties(context, device, NULL, & err);

   // Initialize data on the host    
   generateValues(input_array, ARRAY_SIZE);
   isSorted(input_array, ARRAY_SIZE);
   float sd = calculateSd(input_array, ARRAY_SIZE);
   printf("Standard deviation: %f\n", sd);

   generateValues(input_array2, ARRAY_SIZE);
   isSorted(input_array2, ARRAY_SIZE);
   float sd2 = calculateSd(input_array2, ARRAY_SIZE);
   printf("Standard deviation: %f\n", sd2);

   if (hasCommonValue(input_array, ARRAY_SIZE, input_array2, ARRAY_SIZE)) {
      printf("two arrays have common value(s)\n");
   } else {
      printf("two arrays do not have common value(s)\n");
   }

   // Create input buffer
   cl_mem input_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
      sizeof(int) * ARRAY_SIZE, input_array, & err);
   // Create output buffer
   cl_mem output_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
      sizeof(int) * ARRAY_SIZE, NULL, & err);

   // Load kernel source
   char * source = read_kernel_source("kernels/sort_kernel.cl");

   // Create program from kernel source
   program = clCreateProgramWithSource(context, 1, (const char ** ) & source, NULL, & err);

   // Build program
   err = clBuildProgram(program, 1, & device, NULL, NULL, NULL);

   // Create kernel
   kernel = clCreateKernel(program, "sort", & err);

   // Set kernel arguments
   err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void * ) & input_buffer);
   err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void * ) & output_buffer);
   err = clSetKernelArg(kernel, 2, sizeof(int), (void * ) & ARRAY_SIZE);

   // Enqueue kernel
   size_t global_size = ARRAY_SIZE;
   err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, & global_size, NULL, 0, NULL, NULL);

   // Read results from output buffer
   err = clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, sizeof(int) * ARRAY_SIZE,
      output_array, 0, NULL, NULL);

   // ------ 2nd array --------//

   // Create input buffer
   cl_mem input_buffer2 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
      sizeof(int) * ARRAY_SIZE, input_array2, & err);
   // Create output buffer
   cl_mem output_buffer2 = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
      sizeof(int) * ARRAY_SIZE, NULL, & err);

   // Set kernel arguments
   err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void * ) & input_buffer2);
   err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void * ) & output_buffer2);
   err = clSetKernelArg(kernel, 2, sizeof(int), (void * ) & ARRAY_SIZE);

   // Enqueue kernel
   err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, & global_size, NULL, 0, NULL, NULL);

   // Read results from output buffer
   err = clEnqueueReadBuffer(queue, output_buffer2, CL_TRUE, 0, sizeof(int) * ARRAY_SIZE,
      output_array2, 0, NULL, NULL);


   // Print sorted arrays
   printf("Sorted array1:\n");
   for (int i = 0; i < ARRAY_SIZE; i++) {
      printf("%d ", output_array[i]);
   }
   printf("\n");

   printf("Sorted array2:\n");
   for (int i = 0; i < ARRAY_SIZE; i++) {
      printf("%d ", output_array2[i]);
   }

   // -------- MERGE ARRAYS --------- //
    int ARRAY_SIZE2 = ARRAY_SIZE*2;
   int output_array3[ARRAY_SIZE2];

   // Create input buffer
   input_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
      sizeof(int) * ARRAY_SIZE, output_array, & err);
   input_buffer2 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
      sizeof(int) * ARRAY_SIZE, output_array2, & err);
   // Create output buffer
   cl_mem output_buffer3 = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
      sizeof(int) * ARRAY_SIZE2, NULL, & err);

   // Load kernel source
   char * source2 = read_kernel_source("kernels/merge_sort.cl");

   // Create program from kernel source
   program = clCreateProgramWithSource(context, 1, (const char ** ) & source2, NULL, & err);

   // Build program
   err = clBuildProgram(program, 1, & device, NULL, NULL, NULL);

   // Create kernel
   kernel = clCreateKernel(program, "merge_sort", & err);

   // Set kernel arguments
   err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void * ) & input_buffer);
   err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void * ) & input_buffer2);
   err = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void * ) & output_buffer3);
   err = clSetKernelArg(kernel, 3, sizeof(int), (void * ) & ARRAY_SIZE);

   // Enqueue kernel
   err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, & global_size, NULL, 0, NULL, NULL);

   // Read results from output buffer
   err = clEnqueueReadBuffer(queue, output_buffer3, CL_TRUE, 0, sizeof(int) * ARRAY_SIZE2,
      output_array3, 0, NULL, NULL);

    // print merged array
    printf("Merged array:\n");
   for (int i = 0; i < ARRAY_SIZE2; i++) {
      printf("%d ", output_array3[i]);
   }

   // Cleanup
   clReleaseMemObject(input_buffer);
   clReleaseMemObject(output_buffer);
   clReleaseMemObject(input_buffer2);
   clReleaseMemObject(output_buffer2);
   clReleaseMemObject(output_buffer3);
   clReleaseKernel(kernel);
   clReleaseProgram(program);
   clReleaseCommandQueue(queue);
   clReleaseContext(context);
   free(source);
   free(source2);

   return 0;
}