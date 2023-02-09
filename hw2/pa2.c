#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "hbt.h"
#include "bst.h"

/*static void Convert_File(char* filename) {
    FILE* file;
    file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    int length = ftell(file);

    int size = length / (sizeof(int) + sizeof(char));
    fseek(file, 0, SEEK_SET);

    // Making array to keep track of whether tree has right or left child
    int* temp_int = (int*)malloc(sizeof(int));
    char* temp_char = (char*)malloc(sizeof(char));
    int* int_array = (int*)malloc((size * sizeof(int)));
    char* char_array = (char*)malloc((size * sizeof(char)));

    // Reading the information into an array
    for(int i = 0; i < size; i++) {
        fread(temp_int, sizeof(int), 1, file);
        int_array[i] = temp_int[0];
        fread(temp_char, sizeof(char), 1, file);
        char_array[i] = temp_char[0];
    }

    FILE* output;
    output = fopen("Converted.txt", "w");
    
    for(int j = 0; j < size; j++) {
        fprintf(output, "This is the first value %d\n", int_array[j]);
        fprintf(output, "This is the first char %c\n", char_array[j]); 
    }

    fclose(output);
    free(temp_int);
    free(temp_char);
    free(int_array);
    free(char_array);
    fclose(file);
}*/

int main(int argc, char* argv[]) {

    int valid = -1;
    int valid_bst = 0;
    int valid_hbt = 0;

    if((strcmp(argv[1],"-e")) == 0) {
        char* filename = argv[2];

        valid = Valid_File(filename);

        if(valid == 1) {
            valid_bst = Valid_BST(filename, &valid_hbt);
            printf("%d,%d,%d\n", valid, valid_bst, valid_hbt);
            return EXIT_SUCCESS;
        }
        else {
            printf("%d,%d,%d\n", valid, valid_bst, valid_hbt);
            return EXIT_FAILURE;
        }
    }
    else if((strcmp(argv[1],"-b")) == 0) {
        char* filename = argv[2];
        char* outputFile = argv[3];

        valid = Valid_File(filename);
        bool error = false;

        if(valid == 1) {
            BST_Read(filename, outputFile, &error);

            if(error == false) {
                printf("%d\n", valid);
                return EXIT_SUCCESS;
            }
            else {
                valid = 0;
                printf("%d\n", valid);
                return EXIT_FAILURE;
            }
        }
        else {
            printf("%d\n", valid);
            return EXIT_FAILURE;
        }
    }
}