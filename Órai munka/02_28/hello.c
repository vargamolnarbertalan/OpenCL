#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <CL/cl.h>

const int SAMPLE_SIZE = 1000;

int main(void)
{
    clock_t start_time, end_time;
    double cpu_time_used;

    
    

    char str1[50] = "kernels/";
    char str2[50];
    char str3[50] = ".cl";
    
    
    printf("Select a code! (a,b,c,d)\n");
    scanf("%s", str2);

    strcat(str1, str2);
    strcat(str1, str3);

    //printf(str1);

    FILE *f = fopen(str1, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
    char *kernel_code = malloc(fsize + 1);
    fread(kernel_code, fsize, 1, f);
    fclose(f);
    kernel_code[fsize] = 0;

    //printf("\n");
    //printf(kernel_code);
    //printf("\n");

    int i;
    cl_int err;

    start_time = clock();
    // Get platform
    cl_uint n_platforms;
	cl_platform_id platform_id;
    err = clGetPlatformIDs(1, &platform_id, &n_platforms);
	if (err != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", err);
		return 0;
	}
    end_time = clock(); // capture the end time
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate the CPU time used in seconds
    printf("GET PLATFORM CPU time used: %f seconds\n", cpu_time_used);

    // Get device
    start_time = clock();
	cl_device_id device_id;
	cl_uint n_devices;
	err = clGetDeviceIDs(
		platform_id,
		CL_DEVICE_TYPE_GPU,
		1,
		&device_id,
		&n_devices
	);
	if (err != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", err);
		return 0;
	}
    end_time = clock(); // capture the end time
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate the CPU time used in seconds
    printf("GET DEVICE CPU time used: %f seconds\n", cpu_time_used);

    // Create OpenCL context
    start_time = clock();
    cl_context context = clCreateContext(NULL, n_devices, &device_id, NULL, NULL, NULL);
    end_time = clock(); // capture the end time
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate the CPU time used in seconds
    printf("CREATE OPENCL CPU time used: %f seconds\n", cpu_time_used);

    // Build the program
    start_time = clock();
    cl_program program = clCreateProgramWithSource(context, 1, &kernel_code, NULL, NULL);
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Build error! Code: %d\n", err);
        return 0;
    }
    cl_kernel kernel = clCreateKernel(program, "hello_kernel", NULL);
    end_time = clock(); // capture the end time
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate the CPU time used in seconds
    printf("BUILD PROGRAM CPU time used: %f seconds\n", cpu_time_used);

    // Create the host buffer and initialize it
    start_time = clock();
    int* host_buffer = (int*)malloc(SAMPLE_SIZE * sizeof(int));
    for (i = 0; i < SAMPLE_SIZE; ++i) {
        host_buffer[i] = i;
    }
    end_time = clock(); // capture the end time
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate the CPU time used in seconds
    printf("CREATE HOST BUFFER CPU time used: %f seconds\n", cpu_time_used);

    // Create the device buffer
    start_time = clock();
    cl_mem device_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, SAMPLE_SIZE * sizeof(int), NULL, NULL);
    end_time = clock(); // capture the end time
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate the CPU time used in seconds
    printf("CREATE DEVICE BUFFER CPU time used: %f seconds\n", cpu_time_used);

    // Set kernel arguments
    start_time = clock();
    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&device_buffer);
    clSetKernelArg(kernel, 1, sizeof(int), (void*)&SAMPLE_SIZE);
    end_time = clock(); // capture the end time
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate the CPU time used in seconds
    printf("SET KERNEL ARGS CPU time used: %f seconds\n", cpu_time_used);

    // Create the command queue
    start_time = clock();
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, NULL, NULL);
    end_time = clock(); // capture the end time
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate the CPU time used in seconds
    printf("CREATE COMM QUEUE CPU time used: %f seconds\n", cpu_time_used);

    // Host buffer -> Device buffer
    start_time = clock();
    clEnqueueWriteBuffer(
        command_queue,
        device_buffer,
        CL_FALSE,
        0,
        SAMPLE_SIZE * sizeof(int),
        host_buffer,
        0,
        NULL,
        NULL
    );
    end_time = clock(); // capture the end time
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate the CPU time used in seconds
    printf("HOST DEV BUFF CPU time used: %f seconds\n", cpu_time_used);

    // Size specification
    start_time = clock();
    size_t local_work_size = 256;
    size_t n_work_groups = (SAMPLE_SIZE + local_work_size + 1) / local_work_size;
    size_t global_work_size = n_work_groups * local_work_size;
    end_time = clock(); // capture the end time
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate the CPU time used in seconds
    printf("SIZE SPEC CPU time used: %f seconds\n", cpu_time_used);

    // Apply the kernel on the range
    start_time = clock();
    clEnqueueNDRangeKernel(
        command_queue,
        kernel,
        1,
        NULL,
        &global_work_size,
        &local_work_size,
        0,
        NULL,
        NULL
    );
    end_time = clock(); // capture the end time
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate the CPU time used in seconds
    printf("APPLY KERNEL CPU time used: %f seconds\n", cpu_time_used);

    // Host buffer <- Device buffer
    start_time = clock();
    clEnqueueReadBuffer(
        command_queue,
        device_buffer,
        CL_TRUE,
        0,
        SAMPLE_SIZE * sizeof(int),
        host_buffer,
        0,
        NULL,
        NULL
    );
    end_time = clock(); // capture the end time
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate the CPU time used in seconds
    printf("DEV HOST BUF CPU time used: %f seconds\n", cpu_time_used);

    start_time = clock();
    for (i = 0; i < SAMPLE_SIZE; ++i) {
        printf("[%d] = %d, ", i, host_buffer[i]);
    }
    end_time = clock(); // capture the end time
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // calculate the CPU time used in seconds
    printf("RUNTIME CPU time used: %f seconds\n", cpu_time_used);

    // Release the resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(device_id);

    free(host_buffer);
}
