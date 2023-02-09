#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "shell_array.h"
#include "shell_list.h"


static void _free_node(Node* head) {
    while(head->next != NULL) {
        Node* prev = head;
        Node* next = head->next;
        head = next;
        free(prev);
    }
    free(head); 
}

int main(int argc, char* argv[]) {

    long n_comp = 0;
    
    if((strcmp(argv[1],"-l")) != 0) {
        char* filename = argv[2];
        char* savefile = argv[3];
        int size = 0;

        long* array = Array_Load_From_File(filename, &size);
        for(int i = 0; i < size; i++) {
            printf("%ld ", array[i]);
        }

        Array_Shellsort(array, size, &n_comp);

        size = Array_Save_To_File(savefile, array, size);

        printf("%ld", n_comp);
        free(array);
    }
    else if((strcmp(argv[1],"-l")) == 0) {
        char* filename = argv[2];
        char* savefile = argv[3];

        Node* head = List_Load_From_File(filename);

        //head = List_Shellsort(head, &n_comp);

        List_Save_To_File(savefile, head);

        printf("%ld", n_comp);
        _free_node(head);
    }

    return EXIT_SUCCESS;
}