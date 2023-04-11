__kernel void sort(__global int* input, __global int* output, const int length)
{
    int i, j, min_idx, tmp;
 
    // Iterate over each element of the array
    for (i = 0; i < length-1; i++)
    {
        // Find the minimum element in the unsorted part of the array
        min_idx = i;
        for (j = i+1; j < length; j++)
            if (input[j] < input[min_idx])
                min_idx = j;
 
        // Swap the minimum element with the current element
        tmp = input[min_idx];
        input[min_idx] = input[i];
        input[i] = tmp;
    }

    for(i = 0; i < length; i++){
        output[i] = input[i];
    }
}
