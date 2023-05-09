__kernel void sample_kernel(__global const float *input, __global float *output, const int length, const int filter_size) {
    int i = get_global_id(0);
    float sum = 0.0f;
    
    
    if(i < length){
        for (int j = i - filter_size/2; j <= i + filter_size/2; j++) {
         if (j >= 0 && j < length) {
            sum += input[j];            
        }
        
    }
    printf("input[i]=%f\n",input[i]);
    
    output[i] = sum ;// filter_size;

    printf("output[i]=%f\n",output[i]);
    }
    
}