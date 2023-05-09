#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

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

void exportToCsv(double execution_time, double duration);

int main() {    
    clock_t start, end;
    double cpu_time_used;
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
    double *buffer = (double*) malloc(num_samples * sizeof(double));

    start = clock();

    // Read the audio data from the file
    short sample;
    for (int i = 0; i < num_samples; i++) {
        fread(&sample, sizeof(short), 1, input_file);
        buffer[i] = ((double)sample / 32767.0) / 8.0;  // Divide amplitude by 8
    }
    double duration = num_samples / sample_rate;
    end = clock(); // get end time

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Close the input file
    fclose(input_file);

    // Open the output file for writing
    FILE *output_file = fopen("output_modified.wav", "wb");
    if (output_file == NULL) {
        printf("Error: Failed to open output file\n");
        return 1;
    }

    // Write the WAV file header
    fwrite(&header, sizeof(WavHeader), 1, output_file);

    // Write the modified audio data to the file
    for (int i = 0; i < num_samples; i++) {
        sample = (short) round(buffer[i] * 32767);
        fwrite(&sample, sizeof(short), 1, output_file);
    }

    // Close the output file and free the buffer memory
    fclose(output_file);
    free(buffer);

    printf("WAV file written successfully!\n");

    exportToCsv(cpu_time_used,duration/60);
    
    return 0;
}

void exportToCsv(double execution_time, double duration){
    // Open the output file
    time_t t = time(NULL);
    struct tm *current_time = localtime(&t);

    char datetime_str[25];
    strftime(datetime_str, sizeof(datetime_str), "%Y_%m_%d_%H_%M_%S", current_time);
    printf("Measurement timestamp: %s\n", datetime_str);
    char str2[] = "_output.csv";
    strcat(datetime_str, str2);
    FILE* output_file = fopen(datetime_str, "w");

    // Write the header row
    fprintf(output_file, "wav_duration(min),execution_time(sec)\n");
    int dur = (int) duration;
    // Write the data row
    fprintf(output_file, "%d,%0.3f\n",dur,execution_time);

    // Close the output file
    fclose(output_file);
}