__kernel void modifyAudioData(__global const short* input, __global double* output, int num_samples) {
    
    int global_size = 256;
    int gid = get_global_id(0);

    for(int i = gid; i < num_samples; i += global_size){
        output[i] = ((double)input[i] / 32767.0) / 8.0;
    }

}