#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "shell_list.h"


Node *List_Load_From_File(char *filename) {
    FILE* file;
    file = fopen(filename, "rb");
    Node* head = NULL;

    fseek(file, 0, SEEK_END);
    int length = ftell(file);

    int size = length / sizeof(long);
    fseek(file, 0, SEEK_SET);

    if(size != 0) {
        head = malloc(sizeof(*head));
        fread(&(head->value), sizeof(long), 1, file);
        head->next = NULL;
        Node* node = head;
        for(int i = 1; i < size; i++) {
            Node* new_node = malloc(sizeof(*new_node));
            fread(&(new_node->value), sizeof(long), 1, file);
            new_node->next = NULL;
            node->next = new_node;
            node = node->next;
        }
    }

    fclose(file);
    return(head);
}

int List_Save_To_File(char *filename, Node *list) {
    FILE* file;
    file = fopen(filename, "wb");
    int size = 0;
    Node* start = list;

    if(start != NULL) {
        while(start->next != NULL) {
            Node* next = start->next;
            start = next;
            size++;
        }
        size++;
    }

    for(int i = 0; i < size; i++) {
        if(list != NULL) {
            fwrite(&(list->value), sizeof(long), 1, file);
        }
        if(list->next != NULL) {
            Node* next = list->next;
            list = next;
        }
    }

    fclose(file);
    return size;
}

static long get_value_from_list(Node** a_head, int index) {
    Node* node = *a_head;

    for(int i = 0; i < index; i++) {
        if(node != NULL && node->next != NULL) {
            Node* next = node->next;
            node = next;
        }
    }
    long val = node->value;

    return val;
}

static Node* list_swap_address(int index_one, int index_two, Node** cur_head) {
    Node* first_node = *cur_head;
    Node* second_node = *cur_head;

    Node* first_prev = NULL;
    Node* second_prev = NULL;

    Node* first_after = NULL;
    Node* second_after = NULL;

    for(int i = 0; i < index_one; i++) {
        Node* next_one = first_node->next;
        first_prev = first_node;
        first_node = next_one;
        first_after = first_node->next;
    }

    for(int j = 0; j < index_two; j++) {
        Node* next_two = second_node->next;
        second_prev = second_node;
        second_node = next_two; 
        second_after = second_node->next;
    }

    if((first_prev != NULL) && (second_prev != NULL)) {
        first_prev->next = second_node;
        first_node->next = second_after;
        second_prev->next = first_node;
        second_node->next = first_after;
    }
    else if ((first_prev == NULL) && (second_prev != NULL) && (first_after != second_node))
    {
        *cur_head = second_node;
        (*cur_head)->next = first_after;
        first_node->next = second_after;
        second_prev->next = first_node;
    }
    else if((second_prev == NULL) && (first_prev != NULL) && (second_after != first_node)) {
        *cur_head = first_node;
        (*cur_head)->next = second_after;
        second_node->next = first_after;
        first_prev->next = second_node;
    }
    else if((first_after == second_node) && (first_prev != NULL) && (second_prev != NULL)) {
       first_node->next = second_after;
       second_node->next = first_node;
       first_prev->next = second_node; 
    }
    else if((second_after == first_node) && (first_prev != NULL) && (second_prev != NULL)) {
        second_node->next = first_after;
        first_node->next = second_node;
        second_prev->next = first_node;
    }
    else if((first_after == second_node) && (first_prev == NULL)) {
        cur_head = &second_node;
        first_node->next = second_after;
        second_node->next = first_node;
    }
    else if((second_after == first_node) && (second_prev == NULL) && (first_prev != NULL)) {
        cur_head = &first_node;
        second_node->next = first_after;
        first_node->next = second_node;
    }
    return *cur_head;
}

Node *List_Shellsort(Node *list, long *n_comp) {

    Node* start = list;
    int size = 0;

    if(start != NULL) {
        while(start->next != NULL) {
            Node* next = start->next;
            start = next;
            size++;
        }
        size++;
    }

    int h = 1;
    while (h <= size) {
        int temp = (h * 3) + 1;
        h = temp;
    }

    h = (h - 1) / 3;

    Node* main_head = list;

    for(int k = h; k > 0; k = (k - 1) / 3) {
        for(int j = k; j <= size - 1; j++) {
                int i = j;
                *n_comp += 1;
                while(((get_value_from_list(&main_head, i - k))) > get_value_from_list(&main_head, j) && (i >= k)) {
                    *n_comp += 1;
                    main_head = list_swap_address(i, i - k, &main_head);
                    i = i - k;
                }
                main_head = list_swap_address(i, j, &main_head);
        }
    }
    return main_head;
}