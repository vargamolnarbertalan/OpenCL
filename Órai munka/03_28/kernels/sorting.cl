__kernel void sorting_kernel(__global int* arr, int size)
{
    int i, j;
    for(i = 0; i < size; ++i) {
        for(j = 0; j < size - i - 1; ++j) {
            int a = arr[j];
            int b = arr[j+1];
            if(a > b) {
                arr[j] = b;
                arr[j+1] = a;
            }
        }
    }
    for(int i = 0; i < size; i++){
        printf("%d ",arr[i]);
    }
    
}