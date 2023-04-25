#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    // Specify the signal parameters
    double frequency = 440;     // Hz
    double amplitude = 0.5;     // 0 to 1
    double duration = 5;        // seconds
    int sample_rate = 44100;    // samples per second
    
    // Compute the number of samples
    int num_samples = (int)(duration * sample_rate);
    
    // Allocate memory for the audio buffer
    double *buffer = (double*) malloc(num_samples * sizeof(double));
    
    // Generate the audio signal
    for (int i = 0; i < num_samples; i++) {
        buffer[i] = amplitude * sin(2.0 * M_PI * frequency * i / sample_rate);
    }
    
    // Open the output file for writing
    FILE *file = fopen("output.wav", "wb");
    
    // Write the WAV file header
    WavHeader header;
    header.chunk_id[0] = 'R';
    header.chunk_id[1] = 'I';
    header.chunk_id[2] = 'F';
    header.chunk_id[3] = 'F';
    header.chunk_size = num_samples * sizeof(short) + sizeof(WavHeader) - 8;
    header.format[0] = 'W';
    header.format[1] = 'A';
    header.format[2] = 'V';
    header.format[3] = 'E';
    header.subchunk1_id[0] = 'f';
    header.subchunk1_id[1] = 'm';
    header.subchunk1_id[2] = 't';
    header.subchunk1_id[3] = ' ';
    header.subchunk1_size = 16;
    header.audio_format = 1;
    header.num_channels = 1;
    header.sample_rate = sample_rate;
    header.byte_rate = sample_rate * sizeof(short);
    header.block_align = sizeof(short);
    header.bits_per_sample = 16;
    header.subchunk2_id[0] = 'd';
    header.subchunk2_id[1] = 'a';
    header.subchunk2_id[2] = 't';
    header.subchunk2_id[3] = 'a';
    header.subchunk2_size = num_samples * sizeof(short);
    fwrite(&header, sizeof(WavHeader), 1, file);
    
    // Write the audio data to the file
    for (int i = 0; i < num_samples; i++) {
        short sample = (short) round(buffer[i] * 32767);
        fwrite(&sample, sizeof(short), 1, file);
    }
    
    // Close the file and free the buffer memory
    fclose(file);
    free(buffer);
    
    printf("WAV file written successfully!\n");
    
    return 0;
}
