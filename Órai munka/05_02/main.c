#define CL_TARGET_OPENCL_VERSION 220

#include "kernel_loader.h"
#include "utils.h"
#include <CL/cl.h>

#include <stdio.h>
#include <stdlib.h>

const int SAMPLE_SIZE = 3;

int main(void)
{

    float input[] = {0.1, 0.2, 0.3, 0.4, 0.5};
    int length = 5;
    int N = length;
    int filter_size = SAMPLE_SIZE;
    
    // Allocate memory for the output array
    float *output = (float*) malloc(length * sizeof(float));
    int i;
    cl_int err;
    int error_code;

    // Get platform
    cl_uint n_platforms;
	cl_platform_id platform_id;
    err = clGetPlatformIDs(1, &platform_id, &n_platforms);
	if (err != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", err);
		return 0;
	}

    // Get device
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

    // Create OpenCL context
    cl_context context = clCreateContext(NULL, n_devices, &device_id, NULL, NULL, NULL);

    // Build the program
    const char* kernel_code = load_kernel_source("kernels/sample.cl", &error_code);
    if (error_code != 0) {
        printf("Source code loading error!\n");
        return 0;
    }
    cl_program program = clCreateProgramWithSource(context, 1, &kernel_code, NULL, NULL);
    const char options[] = "";
    err = clBuildProgram(
        program,
        1,
        &device_id,
        options,
        NULL,
        NULL
    );
    if (err != CL_SUCCESS) {
        printf("Build error! Code: %d\n", err);
        size_t real_size;
        err = clGetProgramBuildInfo(
            program,
            device_id,
            CL_PROGRAM_BUILD_LOG,
            0,
            NULL,
            &real_size
        );
        char* build_log = (char*)malloc(sizeof(char) * (real_size + 1));
        err = clGetProgramBuildInfo(
            program,
            device_id,
            CL_PROGRAM_BUILD_LOG,
            real_size + 1,
            build_log,
            &real_size
        );
        // build_log[real_size] = 0;
        printf("Real size : %d\n", real_size);
        printf("Build log : %s\n", build_log);
        free(build_log);
        return 0;
    }
    cl_kernel kernel = clCreateKernel(program, "sample_kernel", NULL);

    // Create the host buffer and initialize it
    cl_mem input_buffer = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        length * sizeof(float),
        NULL,
        &err
    );
    if (err != CL_SUCCESS) {
        printf("Unable to create input buffer! Code: %d\n", err);
        return 0;
    }

    // Create the device buffer
    cl_mem output_buffer = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        length * sizeof(int),
        NULL,
        &err
    );
    if (err != CL_SUCCESS) {
        printf("Unable to create output buffer! Code: %d\n", err);
        return 0;
    }

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&input_buffer);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&output_buffer);
    clSetKernelArg(kernel, 2, sizeof(int), (void *)&N);
    clSetKernelArg(kernel, 3, sizeof(int), (void *)&filter_size);

    // Create the command queue
    cl_command_queue command_queue = clCreateCommandQueue(
        context, device_id, CL_QUEUE_PROFILING_ENABLE, NULL);

    // Host buffer -> Device buffer
    clEnqueueWriteBuffer(
        command_queue,
        input_buffer,
        CL_TRUE,
        0,
        length * sizeof(float),
        input,
        0,
        NULL,
        NULL
    );

    size_t local_size = 256;
    size_t global_size = 256;
    err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error enqueueing kernel: %d\n", err);
        return 1;
    }


    err = clEnqueueReadBuffer(command_queue, output_buffer, CL_TRUE, 0, length * sizeof(float), output, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error reading output buffer: %d\n", err);
        return 1;
    }
    clFinish(command_queue);

    for(int i = 0; i < length; i++){
        printf("%f\n", output[i]);
    }

    // Release resources
    //clReleaseMemObject(input_buffer);
    clReleaseMemObject(output_buffer);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(command_queue);
    }
