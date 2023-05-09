#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];
} ChunkHeader;

typedef struct {
    char subchunk1_id[4];
    uint32_t subchunk1_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
} Subchunk1Header;

typedef struct {
    char subchunk2_id[4];
    uint32_t subchunk2_size;
} Subchunk2Header;

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s [input_file]\n", argv[0]);
        return 1;
    }

    // Open input file for reading
    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        printf("Error: could not open input file '%s'\n", argv[1]);
        return 1;
    }

    // Read in file header
    ChunkHeader chunk_header;
    fread(&chunk_header, sizeof(chunk_header), 1, input_file);

    Subchunk1Header subchunk1_header;
    fread(&subchunk1_header, sizeof(subchunk1_header), 1, input_file);

    Subchunk2Header subchunk2_header;
    fread(&subchunk2_header, sizeof(subchunk2_header), 1, input_file);

    // Determine maximum absolute amplitude in file
    int16_t max_amplitude = 0;
    int16_t sample;
    for (uint32_t i = 0; i < subchunk2_header.subchunk2_size / sizeof(sample); i++) {
        fread(&sample, sizeof(sample), 1, input_file);
        if (abs(sample) > max_amplitude) {
            max_amplitude = abs(sample);
        }
    }

    // Reset file position to beginning of data
    fseek(input_file, sizeof(chunk_header) + sizeof(subchunk1_header) + sizeof(subchunk2_header), SEEK_SET);

    // Open output file for writing
    FILE *output_file = fopen("normalized.wav", "wb");
    if (output_file == NULL) {
        printf("Error: could not open output file\n");
        return 1;
    }

    // Write file header to output file
    fwrite(&chunk_header, sizeof(chunk_header), 1, output_file);
    fwrite(&subchunk1_header, sizeof(subchunk1_header), 1, output_file);
    fwrite(&subchunk2_header, sizeof(subchunk2_header), 1, output_file);

    // Normalize and write samples to output file
    float factor = 1.0 / max_amplitude;
    while (fread(&sample, sizeof(sample), 1, input_file) == 1) {
        int16_t normalized_sample = (int16_t)(sample * factor);
        fwrite(&normalized_sample, sizeof(normalized_sample), 1, output_file);
    }

    // Close files
    fclose(input_file);
    fclose(output_file);

    return 0;
}
