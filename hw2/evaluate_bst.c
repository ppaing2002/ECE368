#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "hbt.h"
#include "bst.h"

int Valid_File(char* filename) {
    bool correct_file = false;
    bool correct_format = false;
    int final;

    // Check if file can be opened
    FILE* file;
    file = fopen(filename, "rb");

    if(file != NULL) {
        fclose(file);
        correct_file = true;
    }

    // Check for file format if it can be opened
    if(correct_file) {
        for(int i = 0; filename[i] != '\0' && correct_format == false; i++) {
            if(filename[i] == '.') {
                if(filename[i + 1] == 'b') {
                    if (filename[i + 2] == '\0') {
                        correct_format = true;
                    }
                }
            }
        } 
    }

    // Conditions for value to return
    if((correct_file == true) && (correct_format == true)) {
        final = 1;
    }
    else if((correct_file == true) && (correct_format == false)) {
        final = 0;
    }
    else {
        final = -1;
    }

    return final;
}

// Helper function to free BST
void Free_BST(Tnode* node)
{
    if (node == NULL) {
        return;
    }
    Free_BST(node->left);
    Free_BST(node->right);
    free(node);
}

static void Insert_Dup_Key(int val, int child, Tnode** node, bool* valid, bool* inserted) {
    if(*node == NULL) {
        //printf("Made new node DUP KEY: %d\n", val);
        *node = malloc(sizeof(**node));
        **node = (Tnode) {.key = val, .balance = child, .left = NULL, .right = NULL};
        *inserted = true;
    }  
    else if(val < (*node)->key) {
       if((*node)->balance == 2 || (*node)->balance == 3) {
            //printf("Current Node: %d ", (*node)->key);
            //printf("Going to left: %d\n", val);
            Insert_Dup_Key(val, child, &((*node)->left), valid, inserted);
       }
    }
    else if(val > (*node)->key) {
        if((*node)->balance == 1 || (*node)->balance == 3) {
            //printf("Current Node: %d ", (*node)->key);
            //printf("Going to right: %d\n", val);
            Insert_Dup_Key(val, child, &((*node)->right), valid, inserted);
       } 
    }
    else{
       if((*node)->balance == 2 || (*node)->balance == 3) {
            //printf("Current Node SAME KEY: %d ", (*node)->key);
            //printf("Going to left SAME KEY: %d\n", val); 
            Insert_Dup_Key(val, child, &((*node)->left), valid, inserted); 
       } 
       else if((*node)->balance == 1 || (*node)->balance == 3) {
            //printf("Current Node SAME KEY: %d ", (*node)->key);
            //printf("Going to right SAME KEY: %d\n", val); 
            Insert_Dup_Key(val, child, &((*node)->right), valid, inserted); 
       } 
    }
}

static void Insert_BST(int val, char child, bool* valid, Tnode** node) {

    if(*node == NULL) {
        //printf("Made new node: %d\n", val);
        *node = malloc(sizeof(**node));
        **node = (Tnode) {.key = val, .balance = child, .left = NULL, .right = NULL};
    }
    else if((val == (*node)->key)) {
        bool inserted = false;
        Insert_Dup_Key(val, child, node, valid, &inserted);
        if(inserted != true) {
            //printf("Sending right of %d\n", (*node)->key);
            Insert_Dup_Key(val, child, &((*node)->right), valid, &inserted);
        }
    }
    else if((val > (*node)->key)) {         // if value is greater than current node 
        if((*node)->balance == 1 || (*node)->balance == 3) {  // if node can have a child on the right or not
            //printf("Current Node: %d ", (*node)->key);
            //printf("Going to right: %d\n", val);
            Insert_BST(val, child, valid, &((*node)->right));
        }
        else{
            //printf("Invalid at Key: %d\n", val);
            *valid = false;
        }
    }
    else if((val < (*node)->key)) {
            if((*node)->balance == 2 || (*node)->balance == 3) {  // if node can have a child on the left or not
            //printf("Current Node: %d ", (*node)->key);
            //printf("Going to left: %d\n", val); 
            Insert_BST(val, child, valid, &((*node)->left)); 
        }
        else{
            //printf("Invalid at Key: %d\n", val);
            *valid = false;
        }
    }
}

// Helper function to print preorder of tree
void Print_Preorder(Tnode *root) {
    if (root != NULL) {
        //printf("%d ", root->key);
        Print_Preorder(root->left);
        Print_Preorder(root->right);
    }
}

static bool Build_BST(int* int_array, char* char_array, int size, int* valid_hbt) {
    
    bool valid = true;
    Tnode* root = malloc(sizeof(*root));
    *root = (Tnode) {.key = int_array[0], .balance = char_array[0], .left = NULL, .right = NULL};

    for(int i = 1; i < size; i++) {
        Insert_BST(int_array[i], char_array[i], &valid, &root);
    }

    Valid_HBT(&root, valid_hbt);

    /*Print_Preorder(root);
    printf("\n");
    printf("Balance ");
    Print_Preorder_Balance(root);
    printf("\n");*/

    Free_BST(root);

    return valid;
}

int Valid_BST(char* filename, int* valid_hbt) {

    // Opening and reading file
    int valid_tree = 0;
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

    // Building a BST
    bool valid = Build_BST(int_array, char_array, size, valid_hbt);

    if(valid == true) {
        valid_tree = 1; 
    }
    else{
        valid_tree = 0;
    }

    // Freeing all the arrays and closing the file
    free(temp_int);
    free(temp_char);
    free(int_array);
    free(char_array);
    fclose(file);
    return valid_tree;
}

void Print_Preorder_Balance(Tnode *root) {
    if(root != NULL) {
        printf("%d ", root->balance);
        Print_Preorder_Balance(root->left);
        Print_Preorder_Balance(root->right);
    }
}

static int Get_Height(Tnode** node) {
    if(*node == NULL) {
        return 0;
    }
    else {
        int left = Get_Height(&((*node)->left));
        int right = Get_Height(&((*node)->right));

        if (left > right) {
            return (left + 1);
        }
        else {
            return (right + 1);
        }
    }
}

static void Set_Balance(Tnode** node, bool* balanced) {
    if(*node != NULL) {
        int balance = Get_Height(&((*node)->left)) - Get_Height(&((*node)->right));
        (*node)->balance = balance;
        if(balance < -1 || balance > 1) {
            *balanced = false;
        }
        Set_Balance(&((*node)->left), balanced);
        Set_Balance(&((*node)->right), balanced); 
    }
}

void Valid_HBT(Tnode** node, int* valid_hbt) {
    bool balanced = true;

    Set_Balance(node, &balanced);

    if(balanced == true) {
        *valid_hbt = 1;
    }
}