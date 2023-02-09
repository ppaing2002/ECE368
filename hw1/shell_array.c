#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "shell_array.h"

long *Array_Load_From_File(char *filename, int *size) {
    FILE* file;
    file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    int length = ftell(file);

    *size = length / sizeof(long);

    fseek(file, 0, SEEK_SET);
    long* array = (long*)malloc(*size * sizeof(long));
    fread(array, sizeof(long), *size, file);

    fclose(file);

    return array;
}

int Array_Save_To_File(char *filename, long *array, int size) {
    FILE* file;
    file = fopen(filename, "wb");
    int num_saved;

    if((array == NULL) || (size == 0)) {
      num_saved = 0;
    }
    else {
        fwrite(array, sizeof(long), size, file);
        num_saved = size;
    }
    fclose(file);
    return num_saved;
}
 
void Array_Shellsort(long *array, int size, long *n_comp) {
    
    int h = 1;

    while (h <= size) {
        int temp = (h * 3) + 1;
        h = temp;
    }

    h = (h - 1) / 3;

    for(int k = h; k > 0; k = ((k - 1) / 3)) {
        for(int j = k; j <= size - 1; j++) {
            long num = array[j];
            int i = j;
            *n_comp += 1;
            while ((i >= k) && (array[i - k] > num)) {
                *n_comp += 1;
                array[i] = array[i - k];
                i = i - k;
            }
            array[i] = num;
        }
    } 
}
