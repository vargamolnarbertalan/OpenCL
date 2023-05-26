#define CL_TARGET_OPENCL_VERSION 220

#include "kernel_loader.h"
#include "utils.h"
#include <CL/cl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Define the WAV file header structure
typedef struct {
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short audio_format;
    short num_channels;
    int sample_rate;
    int byte_rate;
    short block_align;
    short bits_per_sample;
    char subchunk2_id[4];
    int subchunk2_size;
} WavHeader;

int main() {
    // Set up OpenCL
    cl_platform_id platform;
    cl_device_id device;
    cl_command_queue queue;
    cl_int err;
    clock_t start, end;
    double cpu_time_used;

    // Compute the number of samples
    const int sample_rate = 44100;
    // Open the input file for reading
    FILE *input_file = fopen("output.wav", "rb");
    if (input_file == NULL) {
        printf("Error: Failed to open input file\n");
        return 1;
    }

    // Read the WAV file header
    WavHeader header;
    fread(&header, sizeof(WavHeader), 1, input_file);

    // Compute the number of samples
    int num_samples = header.subchunk2_size / sizeof(short);

    // Allocate memory for the audio buffer
    short *buffer = (short*) malloc(num_samples * sizeof(short));
    double *output_audio_buffer = (double*) malloc(num_samples * sizeof(double));

    

    // Read the audio data from the file
    short sample;
    for (int i = 0; i < num_samples; i++) {
        fread(&sample, sizeof(short), 1, input_file);
        buffer[i] = sample;
    }
    double duration = num_samples / sample_rate;

    // Close the input file
    fclose(input_file);

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
    queue = clCreateCommandQueue(
        context, device_id, CL_QUEUE_PROFILING_ENABLE, NULL);

    int error_code;

    // Load and build program
    const char* source = load_kernel_source("kernels/sample.cl", &error_code);
    if (error_code != 0) {
        printf("Source code loading error!\n");
        return 0;
    }

    cl_program program = clCreateProgramWithSource(context, 1, &source, NULL, NULL);

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
    cl_kernel kernel = clCreateKernel(program, "modifyAudioData", NULL);

    // Create memory buffers
    cl_mem input_buffer = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        num_samples * sizeof(double),
        NULL,
        &err
    );
    if (err != CL_SUCCESS) {
        printf("Unable to create input buffer! Code: %d\n", err);
        return 0;
    }

    cl_mem output_buffer = clCreateBuffer(
        context,
        CL_MEM_READ_WRITE,
        num_samples * sizeof(double),
        NULL,
        &err
    );
    if (err != CL_SUCCESS) {
        printf("Unable to create output buffer! Code: %d\n", err);
        return 0;
    }

    // Set the kernel arguments
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&input_buffer);
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&output_buffer);
    err = clSetKernelArg(kernel, 2, sizeof(int), (void *)&num_samples);

    

    // Enqueue write to input buffer
    clEnqueueWriteBuffer(
        queue,
        input_buffer,
        CL_TRUE,
        0,
        num_samples * sizeof(short),
        buffer,
        0,
        NULL,
        NULL
    );

    size_t local_size = 256;
    size_t global_size = 256;

    start = clock();

    // Enqueue kernel for execution
    //size_t global_size = num_samples;
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error enqueueing kernel: %d\n", err);
        return 1;
    }

    // Enqueue read from output buffer
    err = clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, num_samples * sizeof(double), output_audio_buffer, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error reading output buffer: %d\n", err);
        return 1;
    }
    end = clock();
    clFinish(queue);

    // Create the output WAV file
    FILE* output_file = fopen("output_modified.wav", "wb");
    if (output_file == NULL) {
    printf("Error: Failed to open output file\n");
    return 1;
    }

    // Write the WAV file header
    fwrite(&header, sizeof(WavHeader), 1, output_file);

    // Write the modified audio data to the file
    for (int i = 0; i < num_samples; i++) {
        sample = (short) round(output_audio_buffer[i] * 32767);
        fwrite(&sample, sizeof(short), 1, output_file);
    }

    // Close the output file and free the buffer memory
    fclose(output_file);

    printf("WAV file written successfully!\n");

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    exportToCsv(cpu_time_used, duration / 60);
    
    // Release OpenCL resources
    clReleaseMemObject(output_buffer);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);

    return 0;
}