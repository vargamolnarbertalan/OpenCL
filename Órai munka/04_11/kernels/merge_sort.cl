__kernel void merge_sort(__global const int* input_array1,
                          __global const int* input_array2,
                          __global int* output_array,
                          const int n) {
    int i = get_global_id(0);
    int j = 0;
    int k = 0;
    int temp_array[20];

    while (i < n * 2) {
        if (j < n && k < n) {
            if (input_array1[j] <= input_array2[k]) {
                temp_array[i] = input_array1[j];
                j++;
            } else {
                temp_array[i] = input_array2[k];
                k++;
            }
        } else if (j < n) {
            temp_array[i] = input_array1[j];
            j++;
        } else {
            temp_array[i] = input_array2[k];
            k++;
        }
        i++;
    }

    for (i = 0; i < n * 2; i++) {
        output_array[i] = temp_array[i];
    }
}
